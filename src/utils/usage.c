#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../main.h"
#include "../debug.h"

void ShowUsage(void)
{
    /*
     * show the useage info
     */

    /*
    char *version;
    GetCurrentVersion(&version);
    printf("Version: %s", version);
    FreeGetCurrentVersionBuff(version);
    */

    char *usage = "\n"
                  "Usage: dos-tool -u [address] -p [port] -a [mode]\n"
                  "\n"
                  "Example:\n"
                  "./dos-tool-linux -u http://192.168.1.1/login.asp -p 80 --username admin --password-file /path/password.txt --router feixun_fwr_604h (guess the router password)\n"
                  "./dos-tool-linux -u 192.168.1.1 -p 80 -a 1 (syn flood attack target's 80 port)\n"
                  "./dos-tool-linux -u 192.168.1.1 -p 80 -a udp_flood (udp flood attack target's 80 port)\n"
                  "\n"
                  "-u [url or ip]                       specify intent URL or IP address\n"
                  "-p [port]                            specify port"
                  "-a [mode]                            specify attack mode\n"
                  "                                         0    (or use guess_password) brute force attack\n"
                  "                                         1    (or use syn_flood) syn flood attack\n"
                  "                                         2    (or use udp_flood) udp flood attack\n"
                  "                                         3    (or use ack_reflect) ack reflect attack\n"
                  "                                         4    (or use dns_reflect) dns reflect attack\n"
                  "                                         .etc\n"
                  "--username-encrypt [type]            specify that program should encrypt the username (brute force attack use)\n"
                  "                                         0    (or no_encrypt) do not encrypt\n"
                  "                                         1    (or base64_encrypt) use the base64 to encrypt the username\n"
                  "                                         .etc\n"
                  "--password-encrypt [type]            specify that program should encrypt the password (brute force attack use)\n"
                  "                                         0    (or no_encrypt) do not encrypt\n"
                  "                                         1    (or base64_encrypt) use the base64 to encrypt the username\n"
                  "                                         .etc\n"
                  "--random-saddr                       specify the program enable random source address in dos attack (brute force attack not support)\n"
                  "--thread [number]                    specify the attack thread number (default 4)\n"
                  "--username [username]                specify user-provided username (default 'admin')\n"
                  "--username-file [path]               specify user-provided username file\n"
                  "--password [password]                specify user-provided password\n"
                  "--password-file [path]               specify user-provided password file\n"
                  "--random-password-length <length>    specify random password generate length (default 8)\n"
                  "--ip-repeat-time [time]              specify times which random source ip change to next address\n"
                  "--router [model]                     specify router model, which could found in the support_router.md\n"
                  "                                         feixun_fwr_604h\n"
                  "                                         tplink_test\n"
                  "--help                               show this help message again\n"
                  "\n"
                  "";

    printf("%s", usage);
}