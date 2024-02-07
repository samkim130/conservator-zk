//
// Created by Ray Jenkins on 4/25/15.
//

#ifndef CONSERVATOR_CONSERVATORFRAMEWORKFACTORY_H
#define CONSERVATOR_CONSERVATORFRAMEWORKFACTORY_H


#include "ConservatorFramework.h"
#include "zookeeper.h"

class ConservatorFrameworkFactory {

public:
    unique_ptr<ConservatorFramework> newClient(string connectString);
    unique_ptr<ConservatorFramework> newClient(string connectString, int timeout);
    unique_ptr<ConservatorFramework> newClient(string connectionString, int timeout, clientid_t *cid);
    unique_ptr<ConservatorFramework> newClient(string connectString, int timeout, clientid_t * cid, int znode_size);
    unique_ptr<ConservatorFramework> newClient(string connectString, int timeout, clientid_t * cid, int znode_size, watcher_fn_v2 zk_init_watcher_cb);
};

#endif //CONSERVATOR_CONSERVATORFRAMEWORKFACTORY_H
