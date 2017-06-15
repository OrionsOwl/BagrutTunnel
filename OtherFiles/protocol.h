/* 
Commands:
- list_interface
- open < id_t >
- close < id_t >
- communicate < id_t, data >
- query_interface < ifs_index >
*/

#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

typedef enum command_type_e {
	LIST_INTERFACES = 0,
	OPEN_INTERFACE,
	CLOSE_INTERFACE,
	COMMUNICATE_INTERFACE,
	QUERY_INTERFACE,
} command_type_t;

typedef char[4] ip;

typedef struct list_interfaces_cmd_s {
} list_interfaces_cmd_t;

typedef struct computer_id_s {
	ip_t ip;
	int8_t ifs;	
} computer_id_t;

typedef struct open_interface_cmd_s {
	computer_id_t id;
} open_interface_cmd_t;

typedef struct close_interface_cmd_s {
	computer_id_t id;
} close_interface_cmd_t;

typedef struct communicate_cmd_s {
	computer_id_t id;
	char *data;
} communicate_cmd_t;

typedef struct query_interface_s {
	int8_t index;
} query_interface_t;

typedef union data_u {
	list_interfaces_cmd_t list_cmd;
	open_interface_cmd_t open_cmd;
	close_interface_cmd_t close_cmd;
	communicate_cmd_t comm_cmd;
	query_interface_t query_cmd;
} data_t;

typedef struct commands_s {
	int8_t cmd_type;
	data_t data;
} commands_t;

#endif /* _PROTOCOL_H_ */
