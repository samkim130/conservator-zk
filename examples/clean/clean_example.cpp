#include <conservator/ConservatorFrameworkFactory.h>
#include <zookeeper/zookeeper.h>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char** argv){
    string yadayada = (argc == 2)? argv[1] : "test";
    //Create factory for framework
    ConservatorFrameworkFactory factory = ConservatorFrameworkFactory();
    //Create a new connection object to Zookeeper
    unique_ptr<ConservatorFramework> framework = factory.newClient("localhost:2181",10000000);
    //Start the connection
    framework->start();
    //Verify that znode exists in the Zookeeper Server
    auto check = framework->checkExists()->forPath("/foo");
    if(check == ZOK)
    {
      // Delete node
      framework->deleteNode()->forPath("/foo");
    }
    //Verify that znode exists in the Zookeeper Server
    auto check2 = framework->checkExists()->forPath("/yada");
    if(check2 == ZOK)
    {
      // Delete node
      framework->deleteNode()->forPath("/yada");
    }
    return 0;
}
