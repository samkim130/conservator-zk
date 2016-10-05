#include <conservator/ConservatorFrameworkFactory.h>
#include <zookeeper/zookeeper.h>
#include <iostream>
#include <string>

using namespace std;


string state2String(ZOOAPI int state){
  if (state == 0)
    return "CLOSED_STATE";
  if (state == ZOO_CONNECTING_STATE)
    return "CONNECTING_STATE";
  if (state == ZOO_ASSOCIATING_STATE)
    return "ASSOCIATING_STATE";
  if (state == ZOO_CONNECTED_STATE)
    return "CONNECTED_STATE";
 if (state == ZOO_EXPIRED_SESSION_STATE)
    return "EXPIRED_SESSION_STATE";
  if (state == ZOO_AUTH_FAILED_STATE)
    return "AUTH_FAILED_STATE";

  return "INVALID_STATE";
}


void get_watcher_fn(zhandle_t *zh, int type,
                       int state, const char *path,void *watcherCtx) {
    cout << "get watcher function called for path: " << path <<endl;
    cout << "New state is " << state2String(state)<<endl;
    ConservatorFramework* framework = (ConservatorFramework *) watcherCtx;
    // reset the watch
    if(framework->checkExists()->forPath(path)) {
        framework->getData()->withWatcher(get_watcher_fn, watcherCtx)->forPath(path);
    }
}
void exists_watcher_fn(zhandle_t *zh, int type,
                             int state, const char *path,void *watcherCtx) {
    cout << "exists watcher function called" << endl;
    // reset the watch
    ConservatorFramework* framework = (ConservatorFramework *) watcherCtx;
    framework->checkExists()->withWatcher(exists_watcher_fn, watcherCtx)->forPath(path);
}

int main(int argc, char** argv){
    string yadayada = (argc == 2)? argv[1] : "test";
    //Create factory for framework
    ConservatorFrameworkFactory factory = ConservatorFrameworkFactory();
    //Create a new connection object to Zookeeper
    unique_ptr<ConservatorFramework> framework = factory.newClient("localhost:2181",10000);
    //Start the connection
    framework->start();
    //Create a Znode "/foo" with data "test"
    auto response = framework->create()->forPath("/foo",(char *) "test");
    //Verify that it was correctly created (see zookeeper.h at original project for error definitions")
    if(response == ZOK)
    {
      cout<<"Created correctly: "<<endl;
    }
    else
    {
      cout<<"error creating the node"<<endl;
    }
    //Verify that znode exists in the Zookeeper Server
    auto check = framework->checkExists()->forPath("/foo");
    if(check == ZOK)
    {
      cout<<"Foo exists"<<endl;
      framework->setData()->forPath("/foo",(char *) "mod");
    }
    else
    {
      cout<<"Failed creation of foo"<<endl;
    }
    //Get the data for the created path, you could check for exists and then get the data.
    string data = framework->getData()->forPath("/foo");
    cout<<"Retrieved data: "<<data<<endl;
    //Create an ephemeral and sequential node.
    auto ephemeral = framework->create()->withFlags(ZOO_EPHEMERAL|ZOO_SEQUENCE)->forPath("/foo/bar");
    if(ephemeral == ZOK)
    {
      cout<<"Ephemeral created correctly"<<endl;
    }
    else
    {
      cout<<"Error creating ephimeral"<<endl;
    }
    //Get children for a given znode
    vector<string> children = framework->getChildren()->forPath("/foo");
    //Print znode noames
    for(auto &child : children)
    {
      cout<<child<<",";
    }
    
    cout<<endl;
    //Close connection
    framework->close();
    framework->start();

    framework->checkExists()->withWatcher(exists_watcher_fn, (void*) framework.get())->forPath("/foo");
    //Ephemeral node should not longer exist
    vector<string> children2 = framework->getChildren()->forPath("/foo");
    for(auto &child : children2)
    {
      cout<<child<<",";
    }
    cout<<endl;
    //Crate a new znode
    auto response3 = framework->create()->forPath("/yada","test");
    if(response3 == ZOK)
      cout<<"Created correctly"<<endl;
    else
      cout<<"error creating the node "<<response3<<endl;
 
    string realpath;
    //Create a new ephemeral and sequential node, and obtain the realpath which was generated at the Zookeeper server
    auto ephemeral2 = framework->create()->withFlags(ZOO_EPHEMERAL|ZOO_SEQUENCE)->forPath("/yada/"+yadayada,NULL,realpath);
    cout<<"Real path is " << realpath<<endl;
    //Print children to verify that realpath generated was correct
    vector<string> children3 = framework->getChildren()->forPath("/yada");
    for(auto &child : children3)
    {
      cout<<child<<",";
    }
    framework->deleteNode()->forPath("/foo");
    return 0;
}
