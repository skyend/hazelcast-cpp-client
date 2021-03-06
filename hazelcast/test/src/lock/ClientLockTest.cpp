/*
 * Copyright (c) 2008-2018, Hazelcast, Inc. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/**
 * This has to be the first include, so that Python.h is the first include. Otherwise, compilation warning such as
 * "_POSIX_C_SOURCE" redefined occurs.
 */
#include "HazelcastServerFactory.h"

#include "ClientTestSupport.h"
#include "HazelcastServer.h"

#include "hazelcast/util/Util.h"
#include "hazelcast/client/HazelcastClient.h"
#include "hazelcast/client/ClientConfig.h"
#include "hazelcast/client/ILock.h"

namespace hazelcast {
    namespace client {
        namespace test {
            class ClientLockTest : public ClientTestSupport {
            protected:
                virtual void TearDown() {
                    // clear
                    l->forceUnlock();
                }

                static void SetUpTestCase() {
                    instance = new HazelcastServer(*g_srvFactory);
                    clientConfig = new ClientConfig();
                    clientConfig->addAddress(Address(g_srvFactory->getServerAddress(), 5701));
                    client = new HazelcastClient(*clientConfig);
                    l = new ILock(client->getILock("MyLock"));
                }

                static void TearDownTestCase() {
                    delete l;
                    delete client;
                    delete clientConfig;
                    delete instance;

                    l = NULL;
                    client = NULL;
                    clientConfig = NULL;
                    instance = NULL;
                }

                static HazelcastServer *instance;
                static ClientConfig *clientConfig;
                static HazelcastClient *client;
                static ILock *l;
            };

            HazelcastServer *ClientLockTest::instance = NULL;
            ClientConfig *ClientLockTest::clientConfig = NULL;
            HazelcastClient *ClientLockTest::client = NULL;
            ILock *ClientLockTest::l = NULL;

            void testLockLockThread(util::ThreadArgs &args) {
                ILock *l = (ILock *) args.arg0;
                util::CountDownLatch *latch = (util::CountDownLatch *) args.arg1;
                if (!l->tryLock())
                    latch->countDown();
            }

            TEST_F(ClientLockTest, testLock) {
                l->lock();
                util::CountDownLatch latch(1);
                util::StartedThread t(testLockLockThread, l, &latch);

                ASSERT_TRUE(latch.await(5));
                l->forceUnlock();
            }

            void testLockTtlThread(util::ThreadArgs &args) {
                ILock *l = (ILock *) args.arg0;
                util::CountDownLatch *latch = (util::CountDownLatch *) args.arg1;
                if (!l->tryLock()) {
                    latch->countDown();
                }
                if (l->tryLock(5 * 1000)) {
                    latch->countDown();
                }
            }

            TEST_F(ClientLockTest, testLockTtl) {
                l->lock(3 * 1000);
                util::CountDownLatch latch(2);
                util::StartedThread t(testLockTtlThread, l, &latch);
                ASSERT_TRUE(latch.await(10));
                l->forceUnlock();
            }

            void testLockTryLockThread1(util::ThreadArgs &args) {
                ILock *l = (ILock *) args.arg0;
                util::CountDownLatch *latch = (util::CountDownLatch *) args.arg1;
                if (!l->tryLock(2 * 1000)) {
                    latch->countDown();
                }
            }

            void testLockTryLockThread2(util::ThreadArgs &args) {
                ILock *l = (ILock *) args.arg0;
                util::CountDownLatch *latch = (util::CountDownLatch *) args.arg1;
                if (l->tryLock(20 * 1000)) {
                    latch->countDown();
                }
            }

            TEST_F(ClientLockTest, testTryLock) {

                ASSERT_TRUE(l->tryLock(2 * 1000));
                util::CountDownLatch latch(1);
                util::StartedThread t1(testLockTryLockThread1, l, &latch);
                ASSERT_TRUE(latch.await(100));

                ASSERT_TRUE(l->isLocked());

                util::CountDownLatch latch2(1);
                util::StartedThread t2(testLockTryLockThread2, l, &latch2);
                util::sleep(1);
                l->unlock();
                ASSERT_TRUE(latch2.await(100));
                ASSERT_TRUE(l->isLocked());
                l->forceUnlock();
            }

            void testLockForceUnlockThread(util::ThreadArgs &args) {
                ILock *l = (ILock *) args.arg0;
                util::CountDownLatch *latch = (util::CountDownLatch *) args.arg1;
                l->forceUnlock();
                latch->countDown();
            }

            TEST_F(ClientLockTest, testForceUnlock) {
                l->lock();
                util::CountDownLatch latch(1);
                util::StartedThread t(testLockForceUnlockThread, l, &latch);
                ASSERT_TRUE(latch.await(100));
                ASSERT_FALSE(l->isLocked());
            }

            void testStatsThread(util::ThreadArgs &args) {
                ILock *l = (ILock *) args.arg0;
                util::CountDownLatch *latch = (util::CountDownLatch *) args.arg1;
                ASSERT_TRUE(l->isLocked());
                ASSERT_FALSE(l->isLockedByCurrentThread());
                ASSERT_EQ(1, l->getLockCount());
                ASSERT_TRUE(l->getRemainingLeaseTime() > 1000 * 30);
                latch->countDown();
            }

            TEST_F(ClientLockTest, testStats) {
                l->lock();
                ASSERT_TRUE(l->isLocked());
                ASSERT_TRUE(l->isLockedByCurrentThread());
                ASSERT_EQ(1, l->getLockCount());

                l->unlock();
                ASSERT_FALSE(l->isLocked());
                ASSERT_EQ(0, l->getLockCount());
                ASSERT_EQ(-1L, l->getRemainingLeaseTime());

                l->lock(1 * 1000 * 60);
                ASSERT_TRUE(l->isLocked());
                ASSERT_TRUE(l->isLockedByCurrentThread());
                ASSERT_EQ(1, l->getLockCount());
                ASSERT_TRUE(l->getRemainingLeaseTime() > 1000 * 30);

                util::CountDownLatch latch(1);
                util::StartedThread t(testStatsThread, l, &latch);
                ASSERT_TRUE(latch.await(60));
            }
        }
    }
}

