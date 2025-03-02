
#include "ocr_data_lock.h"

#include <cassert>
#include <string>
#include <vector>


namespace dpso::ocr {
namespace {


struct ObserverData {
    std::function<void()> lockAboutToBeCreated;
    std::function<void()> lockRemoved;
};


struct SharedData {
    std::weak_ptr<DataLock> dataLock;
    std::vector<ObserverData*> observerDatas;

    static std::shared_ptr<SharedData> get(
        const char* engineId, const char* dataDir)
    {
        struct CacheEntry {
            std::string engineId;
            std::string dataDir;
            std::weak_ptr<SharedData> sharedData;
        };

        static std::vector<CacheEntry> cache;

        for (const auto& entry : cache) {
            if (entry.engineId != engineId
                    || entry.dataDir != dataDir)
                continue;

            if (auto sd = entry.sharedData.lock())
                return sd;

            assert(false);
        }

        std::shared_ptr<SharedData> sd{
            new SharedData{},
            [
                engineId = std::string{engineId},
                dataDir = std::string{dataDir}]
            (SharedData* sd)
            {
                for (auto iter = cache.begin();
                        iter < cache.end();
                        ++iter)
                    if (iter->engineId == engineId
                            && iter->dataDir == dataDir) {
                        cache.erase(iter);
                        break;
                    }

                delete sd;
            }};

        cache.push_back({engineId, dataDir, sd});

        return sd;
    }
};


}


std::shared_ptr<DataLock> DataLock::get(
    const char* engineId, const char* dataDir)
{
    auto sd = SharedData::get(engineId, dataDir);
    if (auto dataLock = sd->dataLock.lock())
        return dataLock;

    for (auto* observerData : sd->observerDatas)
        if (observerData->lockAboutToBeCreated)
            observerData->lockAboutToBeCreated();

    auto dataLock = std::shared_ptr<DataLock>{
        new DataLock{},
        [sd](DataLock* dataLock)
        {
            if (!dataLock)
                return;

            delete dataLock;

            for (auto* observerData : sd->observerDatas)
                if (observerData->lockRemoved)
                    observerData->lockRemoved();
        }};

    sd->dataLock = dataLock;

    return dataLock;
}


struct DataLockObserver::Impl {
    std::shared_ptr<SharedData> sd;
    ObserverData data;

    Impl(
            const char* engineId,
            const char* dataDir,
            std::function<void()> lockAboutToBeCreated,
            std::function<void()> lockRemoved)
        : sd{SharedData::get(engineId, dataDir)}
        , data{
            std::move(lockAboutToBeCreated), std::move(lockRemoved)
        }
    {
        sd->observerDatas.push_back(&data);
    }

    ~Impl()
    {
        auto& datas = sd->observerDatas;

        for (auto iter = datas.begin(); iter < datas.end(); ++iter)
            if (*iter == &data) {
                datas.erase(iter);
                break;
            }
    }
};


DataLockObserver::DataLockObserver()
    : impl{}
{
}


DataLockObserver::DataLockObserver(
        const char* engineId,
        const char* dataDir,
        std::function<void()> lockAboutToBeCreated,
        std::function<void()> lockRemoved)
    : impl{std::make_unique<Impl>(
        engineId,
        dataDir,
        std::move(lockAboutToBeCreated),
        std::move(lockRemoved))}
{
}


DataLockObserver::~DataLockObserver() = default;
DataLockObserver::DataLockObserver(
    DataLockObserver&&) noexcept = default;
DataLockObserver& DataLockObserver::operator=(
    DataLockObserver&&) noexcept = default;


bool DataLockObserver::getIsDataLocked() const
{
    return impl && !impl->sd->dataLock.expired();
}


}
