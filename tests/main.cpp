
#include <cstdio>
#include <cstdlib>

#include "flow.h"


int main()
{
    int curRunnerNum = 0;

    for (const auto* runner = test::Runner::getFirst();
            runner;
            runner = runner->getNext()) {
        std::printf(
            "%4i/%i: %s\n",
            ++curRunnerNum, test::Runner::getNumRunners(),
            runner->getName());
        // Flush to make sure that failure() messages (written to
        // stderr) are nested under the test name.
        std::fflush(stdout);
        runner->run();
    }

    std::printf("===\n");
    const auto numFailures = test::getNumFailures();
    if (numFailures == 0)
        std::printf("Everything is OK\n");
    else
        std::printf(
            "%i failure%s\n",
            numFailures,
            numFailures > 1 ? "s" : "");

    return numFailures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
