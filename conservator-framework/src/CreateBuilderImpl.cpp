//
// Created by Ray Jenkins on 4/29/15.
//

#include <sstream>
#include <cstring>
#include "CreateBuilderImpl.h"

CreateBuilderImpl::CreateBuilderImpl(zhandle_t *zk) {
    this->zk = zk;
}

int CreateBuilderImpl::forPath(string path) {
    string realPath;
    return this->forPath(path, NULL,realPath);
}

PathableAndWriteable<int> *CreateBuilderImpl::withFlags(int flags) {
    this->flags = flags;
    return (PathableAndWriteable<int> *) this;

}

int CreateBuilderImpl::forPath(string path, const char *data) {
    char buffer[512];
    int length;
    if (data == NULL) {
        length = -1;
    } else {
        length = char_traits<const char>::length(data);
    }
    return  zoo_create(this->zk, path.c_str(), data, length, &ZOO_OPEN_ACL_UNSAFE, flags, buffer, sizeof(buffer) - 1);
}

int CreateBuilderImpl::forPath(string path, const char *data, string &realPath) {
    char buffer[512];
    int length;
    if (data == NULL) {
        length = -1;
    } else {
        length = char_traits<const char>::length(data);
    }
    int rc =  zoo_create(this->zk, path.c_str(), data, length, &ZOO_OPEN_ACL_UNSAFE, flags, buffer, sizeof(buffer) - 1);
    realPath.assign(buffer,std::strlen(buffer));
    return rc;
}
