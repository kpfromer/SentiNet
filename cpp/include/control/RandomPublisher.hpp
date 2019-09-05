/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : RandomPublisher
 * @created     : Friday Aug 30, 2019 16:01:12 MDT
 */

#ifndef RANDOMPUBLISHER_HPP

#define RANDOMPUBLISHER_HPP

#include "control/ZMQControlClient.hpp"


class RandomPublisher : public ZMQControlClient
{
    public:
        RandomPublisher ();
        virtual ~RandomPublisher ();

        void loop();
    private:
        std::string message;
        /* private data */
};

#endif /* end of include guard RANDOMPUBLISHER_HPP */

