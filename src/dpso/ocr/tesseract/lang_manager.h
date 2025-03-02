
#pragma once

#include <memory>

#include "ocr/lang_manager.h"


namespace dpso::ocr::tesseract {


bool hasLangManager();


std::unique_ptr<LangManager> createLangManager(const char* dataDir);


}
