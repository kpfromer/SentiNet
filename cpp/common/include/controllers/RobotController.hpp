



class RobotController
{
  public:

    RobotController();

    virtual void connect(






  private:

    void initialize_ZMQServer(const std::string& bind_addr)
    {
      camera = std::unique_ptr<ZMQerver>(new ZMQServer(bind_addr));
      branch_process(camera);
    }

    void initialize_ZMQSubscriber(const std::string& pub_addr)
    {
      subscriber = std::unique_ptr<ZMQSubscriber>(new ZMQSubscriber(pub_addr));
      branch_process(subscriber);
    }

    void initialize_ZMQPublisher(const std::string& )
    {
      publisher = std::unique_ptr<ZMQPublisher>(new ZMQPublisher(pub_addr));
      branch_process(publisher);
    }

    static void branch_process(NodeInterface* val)
    {
      int childpid;
      if((childpid = fork()) == -1)
      {
        perror("cant fork");
        exit(1);
      } else if(childpid == 0) {
        T->begin();
      } else {
        return;
      }
    }


    
    std::unique_ptr<ZMQServer> camera;
    std::unique_ptr<ZMQSubscriber> subscriber;
    std::unique_ptr<ZMQPublisher> publisher;
