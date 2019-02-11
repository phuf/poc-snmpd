#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <signal.h>
#include "random-value.h"
#include "agent.h"

using namespace std;

static int keep_running;

RETSIGTYPE stop_server(int a) {
    keep_running = 0;
}

int Agent::run(int argc, char** argv) {
    parse_command_line(argc, argv);

    if (syslog_)
        snmp_enable_calllog();
    else
        snmp_enable_stderrlog();

    if (agentx_subagent_) {
        netsnmp_ds_set_boolean(NETSNMP_DS_APPLICATION_ID, NETSNMP_DS_AGENT_ROLE, 1);
    }

    if (background_ && netsnmp_daemonize(1, !syslog))
        exit(1);

    init_agent("poc-snmpd");
    init_randomValue();

    if (!agentx_subagent_) {
        init_vacm();
        init_usm();
    }

    init_snmp("poc-snmpd");

    if (!agentx_subagent_)
        init_master_agent();

    event_loop();
    snmp_shutdown("poc-snmpd");

    return 0;
}

void Agent::parse_command_line(int argc, char** argv) {
    int c;

    while ((c == getopt(argc, argv, "bsx:"))) {
        switch (c) {
        case 'b':
            background_ = 1;
            break;

        case 's':
            syslog_ = 1;
            break;

        case 'x':
            agentx_subagent_ = 1;
            break;
        }
    }
}

void Agent::event_loop() {
    keep_running = 1;
    signal(SIGTERM, stop_server);
    signal(SIGINT, stop_server);

    snmp_log(LOG_INFO, "poc-snmpd is up and running.\n");

    while (keep_running) {
        agent_check_and_process(1);
    }
}