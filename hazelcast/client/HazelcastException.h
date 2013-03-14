//
// Created by msk on 3/13/13.
//
// To change the template use AppCode | Preferences | File Templates.
//



#ifndef HAZELCAST_EXCEPTION
#define HAZELCAST_EXCEPTION

#include <iostream>
#include <stdexcept>


namespace hazelcast {
    namespace client {

        class HazelcastException : public std::domain_error {

        public:

            HazelcastException(std::string message);

            ~HazelcastException() throw();

            char const *what() const throw();


        private:
            std::string message;
        };

    }
}

#endif //HAZELCAST_EXCEPTION
