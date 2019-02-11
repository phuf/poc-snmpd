class Agent {
public:
    int run(int argc, char** argv);

private:
    int background_ = 0;
    int syslog_ = 0;
    int agentx_subagent_ = 0;

    void parse_command_line(int argc, char** argv);

    void event_loop();
};
