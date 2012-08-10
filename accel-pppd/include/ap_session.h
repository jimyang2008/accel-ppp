#ifndef __AP_SESSION_H__
#define __AP_SESSION_H__

#define AP_SESSIONID_LEN 16
#define AP_IFNAME_LEN 16

#define AP_STATE_STARTING  1
#define AP_STATE_ACTIVE    2
#define AP_STATE_FINISHING 3
#define AP_STATE_RESTORE   4

#define TERM_USER_REQUEST 1
#define TERM_SESSION_TIMEOUT 2
#define TERM_ADMIN_RESET 3
#define TERM_USER_ERROR 4
#define TERM_NAS_ERROR 5
#define TERM_NAS_REQUEST 6
#define TERM_NAS_REBOOT 7
#define TERM_AUTH_ERROR 8
#define TERM_LOST_CARRIER 9
#define TERM_IDLE_TIMEOUT 10

#define CTRL_TYPE_PPTP  1
#define CTRL_TYPE_L2TP  2
#define CTRL_TYPE_PPPOE 3
#define CTRL_TYPE_IPOE  4

#define MPPE_UNSET   -2
#define MPPE_ALLOW   -1
#define MPPE_DENY    0
#define MPPE_PREFER  1
#define MPPE_REQUIRE 2

struct ap_session;
struct backup_data;

struct ap_ctrl
{
	struct triton_context_t *ctx;
	int type;
	const char *name;
	int max_mtu;
	int mppe;
	char *calling_station_id;
	char *called_station_id;
	int dont_ifcfg:1;
	void (*started)(struct ap_session*);
	void (*finished)(struct ap_session *);
	void (*terminate)(struct ap_session *, int hard);
};

struct ap_private
{
	struct list_head entry;
	void *key;
};

struct ap_session
{
	struct list_head entry;

	int state;
	char *chan_name;
	char ifname[AP_IFNAME_LEN];
	int unit_idx;
	int ifindex;
	char sessionid[AP_SESSIONID_LEN+1];
	time_t start_time;
	time_t stop_time;
	char *username;
	struct ipv4db_item_t *ipv4;
	struct ipv6db_item_t *ipv6;
	char *ipv4_pool_name;
	char *ipv6_pool_name;

	struct ap_ctrl *ctrl;

#ifdef USE_BACKUP
	struct backup_data *backup;
#endif

	int terminating:1;
	int terminated:1;
	int terminate_cause;

	struct list_head pd_list;
};

struct ap_session_stat
{
	unsigned int active;
	unsigned int starting;
	unsigned int finishing;
};


extern pthread_rwlock_t ses_lock;
extern struct list_head ses_list;
extern int ap_shutdown;
extern int sock_fd; // internet socket for ioctls
extern int sock6_fd; // internet socket for ioctls
extern int urandom_fd;
extern struct ap_session_stat ap_session_stat;

void ap_session_init(struct ap_session *ses);
int ap_session_starting(struct ap_session *ses);
void ap_session_finished(struct ap_session *ses);
void ap_session_terminate(struct ap_session *ses, int cause, int hard);
void ap_session_activate(struct ap_session *ses);

void ap_session_ifup(struct ap_session *ses);
void ap_session_ifdown(struct ap_session *ses);

void ap_shutdown_soft(void (*cb)(void));

#endif