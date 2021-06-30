#ifndef I_WRAPPER_H
#define I_WRAPPER_H

#include <map>

namespace ih
{
namespace wrapper
{
class IWrapper
{
public:
    explicit IWrapper(std::map<uint32_t, std::string> const &inputData) :
            m_inputData(inputData)
    {}

    const std::map<uint32_t, std::string> &getInputData() const
    {
        return m_inputData;
    }

private:
    std::map<uint32_t, std::string> const m_inputData;
};
}
}

#endif
