#ifndef CONSOLE_HELPER_H
#define CONSOLE_HELPER_H

#include "CsvReader.h"

#include <vector>

void setProjectFolder(int argc, char* argv[]);
void waitBeforeExit();
void printMenu();
void addPackageFromUser(
    std::vector<Package>& packages,
    const std::vector<Warehouse>& warehouses,
    const std::vector<Stop>& stops
);

#endif
