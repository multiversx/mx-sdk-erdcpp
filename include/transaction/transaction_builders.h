#ifndef ERD_TRANSACTION_BUILDER_ESDT_H
#define ERD_TRANSACTION_BUILDER_ESDT_H

#include "token_payment.h"
#include "itransaction_builder.h"
#include "transaction/transaction.h"
#include "transaction_builder_input.h"

// Forward declaration of tx factory. This will be friend class to each
// tx builder class, in order to access their private constructor.
// Only TransactionFactory will be public and its users will not have access to any
// concrete tx builders anyway, since the factory only returns pure abstract classes.
class TransactionFactory;

class TransactionEGLDTransferBuilder : public ITransactionBuilder
{
    friend class TransactionFactory;

public:
    Transaction build() override;

private:
    explicit TransactionEGLDTransferBuilder(TransactionBuilderInput txInput);

    TransactionBuilderInput m_txInput;
};


class TransactionESDTBuilder : public ITokenTransactionBuilder
{
    friend class TransactionFactory;

public:
    Transaction build() override;

private:
    explicit TransactionESDTBuilder(TransactionBuilderInput txInput, TokenPayment payment);

    TransactionBuilderInput m_txInput;
    TokenPayment m_tokenPayment;
};


#endif //ERD_TRANSACTION_BUILDER_ESDT_H