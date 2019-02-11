#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

int handle_randomValue(
    netsnmp_mib_handler *handler,
    netsnmp_handler_registration *reginfo,
    netsnmp_agent_request_info *reqinfo,
    netsnmp_request_info *requests) {
        
    int random_value = rand();

    switch (reqinfo->mode) {
        case MODE_GET:
            snmp_set_var_typed_value(
                requests->requestvb, 
                ASN_INTEGER,
                (u_char*)&random_value, 
                sizeof(random_value));
            break;

        default:
        {
            snmp_log(
                LOG_ERR,
                "Unknown mode (%d) in handle_dot1dBaseType\n",
                reqinfo->mode);
            
            return SNMP_ERR_GENERR;
        }
    }

    return SNMP_ERR_NOERROR;
}

void init_randomValue() {
    static oid randomValue_oid[] = {1, 3, 6, 1, 4, 1, 8072, 2, 4, 1, 1, 2};

    netsnmp_register_scalar(netsnmp_create_handler_registration(
        "randomValue",
        handle_randomValue,
        randomValue_oid,
        OID_LENGTH(randomValue_oid),
        HANDLER_CAN_RONLY));
}
