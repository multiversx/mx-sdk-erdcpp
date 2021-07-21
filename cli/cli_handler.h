#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include "inputhandler/ext.h"

namespace cli
{
void init();

void handleRequest(ih::ArgParsedResult const &parsedResult);

void handleCreateSignedTransaction(cxxopts::ParseResult const &result);

void handleIssueESDT(cxxopts::ParseResult const &result);

void handleTransferESDT(cxxopts::ParseResult const &result);
}

#endif
