//
// Created by sancar koyunlu on 6/4/13.
// Copyright (c) 2013 hazelcast. All rights reserved.



#ifndef HAZELCAST_PARTITION_RESPONSE
#define HAZELCAST_PARTITION_RESPONSE

#include "../Address.h"
#include "DataSerializable.h"
#include <vector>

namespace hazelcast {
    namespace client {
        namespace impl {
            class PartitionsResponse : public DataSerializable{
            public:
                PartitionsResponse();

                PartitionsResponse(std::vector<Address>& addresses, std::vector<int>& ownerIndexes);

                std::vector<Address> getMembers() const;

                std::vector<int> getOwnerIndexes() const;

                bool isEmpty();

                int getFactoryId() const;

                int getClassId() const;

                void writeData(serialization::BufferedDataOutput& writer);

                void readData(serialization::BufferedDataInput& reader);

            private:
                std::vector<Address> members;
                std::vector<int> ownerIndexes;
            };

        }
    }
}
#endif //HAZELCAST_PARTITION_RESPONSE
