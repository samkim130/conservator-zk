# Overview

This is a fork of the project https://github.com/rjenkins/conservator + some update by gatech staff.

Conservator is a Zookeeper Client library written in C++. It's not a direct port of [Apache Curator](http://curator.apache.org/) but tries to mirror the simplicity and fluent nature of the [CuratorFramework](http://curator.apache.org/apidocs/) API.

This repo contains the build set up for macOS using homebrew as well as minor additional watcher features for default watch when `zookeeper_init` is called.
This is for better error handling when clients encounter network partition and client doesn't realize it has been disconnected until the watcher is triggered upon reconnect.

Read more [here](https://zookeeper.apache.org/doc/current/zookeeperProgrammers.html). Quote from ZK site:
```
...

Example state transitions for an expired session as seen by the expired session's watcher:

'connected' : session is established and client is communicating with cluster (client/server communication is operating properly)
.... client is partitioned from the cluster
'disconnected' : client has lost connectivity with the cluster
.... time elapses, after 'timeout' period the cluster expires the session, nothing is seen by client as it is disconnected from cluster
.... time elapses, the client regains network level connectivity with the cluster
'expired' : eventually the client reconnects to the cluster, it is then notified of the expiration

...
```

## Installing

Instructions for installing with a fresh trusty64 instance

* Download latest [zookeeper](https://zookeeper.apache.org/)
* Install [ant](http://ant.apache.org/) and [openjdk-7-jdk](http://openjdk.java.net/)

```
sudo apt-get install ant
sudo apt-get install openjdk-7-jdk
```

Follow instructions for installing the Zookeeper C Bindings. https://zookeeper.apache.org/doc/r3.4.6/zookeeperProgrammers.html#C+Binding

```
ant compile_jute
cd src/c
sudo apt-get install autoconf
sudo apt-get install libcppunit-dev
sudo apt-get install libtool
autoreconf -if
./configure
make; make install
```
This will install the zookeeper libraries into /usr/local/lib
```
vagrant@vagrant-ubuntu-trusty-64:~/zookeeper-3.4.6/src/c$ ls /usr/local/lib/ | grep zoo
libzookeeper_mt.a
libzookeeper_mt.la
libzookeeper_mt.so
libzookeeper_mt.so.2
libzookeeper_mt.so.2.0.0
libzookeeper_st.a
libzookeeper_st.la
libzookeeper_st.so
libzookeeper_st.so.2
libzookeeper_st.so.2.0.0
```
* Install [libcheck](http://check.sourceforge.net/)
```
sudo apt-get install check
```
* Install the latest version of [CMake](http://www.cmake.org/download/)

## Building

To build simply cd into conservator run cmake . and make and libconservator-framework.a will be installed in the build directory.
```
rjenkins@rjenkins-VirtualBox:~/conservator$ cmake .
-- Configuring done
-- Generating done
-- Build files have been written to: /home/rjenkins/conservator
rjenkins@rjenkins-VirtualBox:~/conservator$ make
[  9%] Building CXX object CMakeFiles/conservator-framework.dir/conservator-framework/src/ConservatorFramework.cpp.o
[ 18%] Building CXX object CMakeFiles/conservator-framework.dir/conservator-framework/src/ConservatorFrameworkFactory.cpp.o
[ 27%] Building CXX object CMakeFiles/conservator-framework.dir/conservator-framework/src/GetDataBuilderImpl.cpp.o
[ 36%] Building CXX object CMakeFiles/conservator-framework.dir/conservator-framework/src/ExistsBuilderImpl.cpp.o
[ 45%] Building CXX object CMakeFiles/conservator-framework.dir/conservator-framework/src/CreateBuilderImpl.cpp.o
[ 54%] Building CXX object CMakeFiles/conservator-framework.dir/conservator-framework/src/DeleteBuilderImpl.cpp.o
[ 63%] Building CXX object CMakeFiles/conservator-framework.dir/conservator-framework/src/SetDataBuilderImpl.cpp.o
[ 72%] Building CXX object CMakeFiles/conservator-framework.dir/conservator-framework/src/GetChildrenBuilderImpl.cpp.o
[ 81%] Building CXX object CMakeFiles/conservator-framework.dir/conservator-framework/src/GetACLBuilderImpl.cpp.o
[ 90%] Building CXX object CMakeFiles/conservator-framework.dir/conservator-framework/src/SetACLBuilderImpl.cpp.o
Linking CXX static library build/libconservator-framework.a
[ 90%] Built target conservator-framework
[100%] Building CXX object conservator-framework/tests/CMakeFiles/ConservatorFrameworkFactoryTest.dir/ConservatorFrameworkFactoryTest.cpp.o
Linking CXX executable ../../build/ConservatorFrameworkFactoryTest
[100%] Built target ConservatorFrameworkFactoryTest
```

## Testing

In order to run unit tests run make test.
```
rjenkins@rjenkins-VirtualBox:~/conservator$ make test
Running tests...
Test project /home/rjenkins/conservator
    Start 1: ConservatorFrameworkFactoryTest
1/1 Test #1: ConservatorFrameworkFactoryTest ...   Passed    5.19 sec

100% tests passed, 0 tests failed out of 1

Total Test time (real) =   5.19 sec
```

## Usage and API

To use simply link to the static ```libconservator-framework.a``` library. 

You can obtain a ConservatorFramework instance by using the ConservatorFrameworkFactory.

```
ConservatorFrameworkFactory factory = ConservatorFrameworkFactory();
ConservatorFramework framework = factory.newClient("localhost:2181");
framework.start();
```

Make sure to close the framework when you're done with the ```framework.close()``` command.

Once you've obtained a ConservatorFramework instance all of the standard zookeeper related operations are availble from this interface, with optional parameters being added with additional fluent calls.

```c++
unique_ptr<GetDataBuilder<string>> getData();
unique_ptr<ExistsBuilder<int>> checkExists();
unique_ptr<CreateBuilder<int>> create();
unique_ptr<DeleteBuilder<int>> deleteNode();
unique_ptr<SetDataBuilder<int>> setData();
unique_ptr<GetChildrenBuilder<string>> getChildren();
unique_ptr<GetACLBuilder<int>> getACL(ACL_vector *vector);
unique_ptr<SetACLBuilder<int>> setACL(ACL_vector *vector);
```

Below are some examples, if you don't see your use case here be sure to read the unit tests contained with [ConservatorFrameworkFactoryTest](https://github.com/rjenkins/conservator/blob/master/conservator-framework/tests/ConservatorFrameworkFactoryTest.cpp) they are fairly comprehensive and provide example of how to use the entire API.

* Creating a znode
```c
 ConservatorFrameworkFactory factory = ConservatorFrameworkFactory();
 ConservatorFramework framework = factory.newClient("localhost:2181");
 framework.start();
 int result = framework.create()->forPath("/foo", (char *) "bar");
```

* Creating an ephemeral znode
```c
ConservatorFrameworkFactory factory = ConservatorFrameworkFactory();
ConservatorFramework framework = factory.newClient("localhost:2181");
framework.start();
ck_assert_int_eq(ZOK, framework.create()->withFlags(ZOO_EPHEMERAL)->forPath("/foo"));
```

* Checking for the existance of a znode
```c
ConservatorFrameworkFactory factory = ConservatorFrameworkFactory();
ConservatorFramework framework = factory.newClient("localhost:2181");
framework.start();
framework.create()->forPath("/foo");
ck_assert_int_eq(ZOK, framework.checkExists()->forPath("/foo"));
```

* Getting the contents of a znode 
```c
ConservatorFrameworkFactory factory = ConservatorFrameworkFactory();
ConservatorFramework framework = factory.newClient("localhost:2181");
framework.start();
framework.create()->forPath("/foo", (char *) "bar");
ck_assert_str_eq("bar", framework.getData()->forPath("/foo").c_str());
```

* Getting a znode and setting a watch
```c
void get_watcher_fn(zhandle_t *zh, int type,
                       int state, const char *path,void *watcherCtx) {
    cout << "get watcher function called" << endl;
    get_watcher_called++;
    // reset the watch
    ConservatorFramework* framework = (ConservatorFramework *) watcherCtx;
    if(framework->checkExists()->forPath(path)) {
        framework->getData()->withWatcher(get_watcher_fn, framework)->forPath(path);
    }
}

START_TEST(framework_getdata_with_watch) {
    ConservatorFrameworkFactory factory = ConservatorFrameworkFactory();
    ConservatorFramework framework = factory.newClient("localhost:2181");
    framework.start();
    framework.create()->forPath("/foo", (char *) "bar");
    string r = framework.getData()->withWatcher(get_watcher_fn, ((void *) &framework))->forPath("/foo");
    ck_assert_str_eq("bar", r.c_str());
    framework.setData()->forPath("/foo", (char *) "bar");
    ck_assert_int_eq(1, get_watcher_called);
}
END_TEST
```

* Setting a znode and setting a znode while checking for version modifications
```c
START_TEST(framework_setdata) {
    ConservatorFrameworkFactory factory = ConservatorFrameworkFactory();
    ConservatorFramework framework = factory.newClient("localhost:2181");
    framework.start();
    framework.create()->forPath("/foo", (char *) "bar");
    ck_assert_int_eq(ZOK, framework.setData()->forPath("/foo", (char *) "moo"));
    ck_assert_str_eq("moo", framework.get("/foo").c_str());
}
END_TEST

START_TEST(framework_setdata_version) {
    ConservatorFrameworkFactory factory = ConservatorFrameworkFactory();
    ConservatorFramework framework = factory.newClient("localhost:2181");
    framework.start();
    framework.create()->forPath("/foo");
    struct Stat stat;
    framework.getData()->storingStatIn(&stat)->forPath("/foo");
    ck_assert_int_eq(ZBADVERSION, framework.setData()->withVersion(100)->forPath("/foo", (char *) "bar"));
    ck_assert_int_eq(ZOK, framework.setData()->withVersion(stat.version)->forPath("/foo", (char *) "bar"));
}
END_TEST
```
* Deleting a znode or deleting a znode and all it's children.
```c
START_TEST(framework_getdata) {
    ConservatorFrameworkFactory factory = ConservatorFrameworkFactory();
    ConservatorFramework framework = factory.newClient("localhost:2181");
    framework.start();
    framework.create()->forPath("/foo", (char *) "bar");
    ck_assert_str_eq("bar", framework.getData()->forPath("/foo").c_str());
}
END_TEST

START_TEST(framework_delete_children) {
    ConservatorFrameworkFactory factory = ConservatorFrameworkFactory();
    ConservatorFramework framework = factory.newClient("localhost:2181");
    framework.start();
    framework.create()->forPath("/flintstones");
    framework.create()->forPath("/flintstones/fred");
    framework.create()->forPath("/flintstones/barney");
    vector<string> children = framework.getChildren()->forPath("/flintstones");
    ck_assert_int_eq(2, children.size());
    ck_assert_str_eq("barney", children.at(0).c_str());
    ck_assert_str_eq("fred", children.at(1).c_str());
    framework.deleteNode()->deletingChildren()->forPath("/flintstones");
    ck_assert_int_eq(ZNONODE, framework.checkExists()->forPath("/flintstones"));

}
END_TEST
```

* Getting a znode's children
```c
START_TEST(framework_getchildren)
{
    ConservatorFrameworkFactory factory = ConservatorFrameworkFactory();
    ConservatorFramework framework = factory.newClient("localhost:2181");
    framework.start();
    framework.create()->forPath("/flintstones");
    framework.create()->forPath("/flintstones/fred");
    framework.create()->forPath("/flintstones/barney");
    vector<string> children = framework.getChildren()->forPath("/flintstones");
    ck_assert_int_eq(2, children.size());
    ck_assert_str_eq("barney", children.at(0).c_str());
    ck_assert_str_eq("fred", children.at(1).c_str());
}
END_TEST
```
## TODO

* Add support for the zookeeper.h zoo_a* asynchronous calls
* Add support for the zoo_*2 calls that allow for setting Stat structs.
* Add support for blocking on connect for framework.start()
* Better error detection and recovery handling.
