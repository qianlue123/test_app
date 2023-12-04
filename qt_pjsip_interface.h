/* $Id$ */
/*
 * Copyright (C) 2008-2011 Teluu Inc. (http://www.teluu.com)
 * Copyright (C) 2003-2008 Benny Prijono <benny@prijono.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <pjsua-lib/pjsua.h>
#include "pjsua_app_common.h"
/** UI Command **/
#ifndef QT_PJSIP_INTERFACE_H
#define QT_PJSIP_INTERFACE_H
typedef struct {
int i; // 与 class1 类中变量一致
int j;
}struct1;

typedef struct qt_call_info_s
{
    /** Call identification. */
    pjsua_call_id	id;

    /** The account ID where this call belongs. */
    pjsua_acc_id	acc_id;

    char	local_info[PJSIP_MAX_URL_SIZE];
	
	char	remote_info[PJSIP_MAX_URL_SIZE];

	int hold_flg;
	

    /** Call state */
    pjsip_inv_state	state;

    /** Text describing the state */
    char		state_text[300];

    /** Last status code heard, which can be used as cause code */
    pjsip_status_code	last_status;

    /** The reason phrase describing the status. */
    char		last_status_text[300];

    /** Up-to-date call connected duration (zero when call is not 
     *  established)
     */
    //if need ,can provide
   // pj_time_val		connect_duration;

    /** Total call duration, including set-up time */
    //pj_time_val		total_duration;

} qt_call_info;


typedef struct qt_acc_info_s
{
    /** 
     * The account ID. 
     */
    int 	id;

    /**
     * Flag to indicate whether this is the default account.
     */
    int		is_default;
	
	int acc_pos;

    /** 
     * Account URI 
     */
    char	phonenumber[PJSIP_MAX_URL_SIZE]		;

    /** 
     * Flag to tell whether this account has registration setting
     * (reg_uri is not empty).
     */
    int		has_registration;

} qt_acc_info;



typedef struct qt_ua_gui_info_s
{
	int call_cnt;
	int conference_state;
	qt_call_info currentcall_info;
	qt_acc_info    accout_arry[10];
	//only for conference
	qt_call_info call_infos[3];
}qt_ua_info;

#ifdef __cplusplus
extern "C" {
#endif
int qt_sip_main(void);
 void qt_make_new_call(char * calledname);
 void qt_make_multi_call(char * calledname);
 void qt_detect_nat_type();
 void qt_send_instant_message(char * calledname,char * msgtext);
 void qt_answer_call();
 void qt_reject_call();
 void qt_forwarding_call(char *nextphone);
//only in 3XX ,should have a nextphone
//the 
 void qt_answer_call_inner(int st_code,char *nextphone);
//1 hangup all  0 current call
 void qt_hangup_call(int ha);
 void qt_cycle_dialog(int direct);
//index or id ???
 int  qt_cycle_account(int accid);
 void qt_add_account(char *uname,char *registrar,char *realm,char * passwd);
 void qt_delete_account(int accid);
 void qt_call_hold();
//call resume
 void qt_call_reinvite();
 void qt_send_update();
/*
 * Change codec priorities.
 */
 void ui_manage_codec_prio();
 void qt_call_transfer(pj_bool_t no_refersub,char *calledname);
 void qt_call_transfer_inner(pj_bool_t no_refersub,char *calledname);
 void ui_call_transfer_replaces(pj_bool_t no_refersub);
 void qt_send_dtmf_2833(char dtnf);
 void qt_send_dtmf_info(char dtnf);
 void qt_send_dtmf_inband(char dtnf);
// void qt_send_arbitrary_request( char *text,char *buf);
// void qt_echo(int echoflg);
// void qt_sleep(int delay);

//use for disable or enable registering of current accunt
 void qt_set_register(int regflg);

//invite the third party to current call to make a three-way conference 
 void qt_conference_add_third_party(char * calledname);
//all members are ready, start conference
 void qt_start_three_party_conference();
/*
 * List the ports in conference bridge,for meidia only！！！
 */
 void qt_conf_list();
/*
 * connect the ports in conference bridge,for meidia only！！！
 */
 int qt_conf_connect(int src_port, int dst_port,int connectflag /*0:disconn,1 conn*/);
 void qt_adjust_volume(int direct);
 void qt_dump_call_quality();
 void qt_dump_configuration();
 void qt_write_settings(const char *filename);
 void qt_get_curr_callinfo( pjsua_call_info *call_info);
/*
 * Dump application states.
 */
 void qt_app_dump(pj_bool_t detail);
 void qt_call_redirect(int opt_redirect);
 void qt_handle_ip_change();

#ifdef __cplusplus
}
#endif

#endif
