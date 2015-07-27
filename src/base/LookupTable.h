#ifndef SQUID_LOOKUPTABLE_H_
#define SQUID_LOOKUPTABLE_H_

#include "SBuf.h"

#include <map>

/**
 * SBuf -> enum lookup table
 *
 * How to use:
 * enum enum_type { ... };
 * static const LookupTable<enum_type>::Record initializerTable[] {
 *   {"key1", ENUM_1}, {"key2", ENUM_2}, ... {nullptr, ENUM_INVALID_VALUE}
 * };
 * LookupTable<enum_type> lookupTableInstance(ENUM_INVALID_VALUE, initializerTable);
 *
 * then in the code:
 * SBuf s(string_to_lookup);
 * enum_type item = lookupTableInstance.lookup(s);
 * if (item != ENUM_INVALID_VALUE) { // do stuff }
 *
 */
template<typename EnumType>
class LookupTable
{
public:
    /// element of the lookup table initialization list
    typedef struct {
        const char *name;
        EnumType id;
    } Record;

    LookupTable(const EnumType theInvalid, const Record data[]) :
        invalidValue(theInvalid)
    {
        for (auto i = 0; data[i].name != nullptr; ++i) {
            lookupTable[SBuf(data[i].name)] = data[i].id;
        }
    }
    EnumType lookup(const SBuf &key) const {
        auto r = lookupTable.find(key);
        if (r == lookupTable.end())
            return invalidValue;
        return r->second;
    }

private:
    typedef std::map<const SBuf, EnumType> lookupTable_t;
    lookupTable_t lookupTable;
    EnumType invalidValue;
};

#endif /* SQUID_LOOKUPTABLE_H_ */