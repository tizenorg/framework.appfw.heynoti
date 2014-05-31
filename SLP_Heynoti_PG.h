/*
 * heynoti
 *
 * Copyright (c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: Jayoun Lee <airjany@samsung.com>, Sewook Park <sewook7.park@samsung.com>,
 * Jaeho Lee <jaeho81.lee@samsung.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */



/**
 *
 * @ingroup   SLP_PG
 * @defgroup   HEYNOTI_PG HEY(ligHt Easy speedY) Notification
@{
<h1 class="pg">Introduction</h1>

<p>Heynoti is notification system which uses the Linux kernel inotify subsystem. </p>
<p> It is ligHt, Easy, speedY, so that is named 'Heynoti'. </p>
<p> But it doesn't support data delivery, is just sent event notification. </p>
<p> If developers want to send event with data like integer, string, they have to use DBus for IPC. </p>>
<p> Heynoti is based on inotify in Linux Sytem. So for more information about inotify, refer to manpage(inotify(7)).</p>

<h1 class="pg">Features</h1>
<ul>
	<li>HEYNOTI provides a mechanism to define/undefine a notification </li>
	<li>It provides a mechanism to subscribe for a notification </li>
	<li>It supports unsubscription from subscription</li>
	<li>It provides a mechanism to publish a notification</li>
</ul>

<h2 class="pg">Properties</h2>
	<ul>
		<li> light weight Notification system. </li>
		<li> Very fast Notifcation delivery </li>
		<li> Convenient API's to use </li>
		<li> It does not send event data </li>
	</ul>

<h1 class="pg"> Heynoti Logical view diagram </h1>
\image html SLP_heynoti_PG_images_logical_view.png "Picture 1. Logical view"


<h1 class="pg"> Heynoti Functional diagram </h1>
\image html SLP_Heynoti_PG_image01.png "Picture 2. Functional  view"


<h1 class="pg">Programming Guide</h1>
<h2 class="pg"> Header file </h2>
<p> header file name :<strong> heynoti.h </strong> </p>

<p>For receiving notification, we can use heynoti_attach_handler() or heynoti_poll_event().
heynoti_attach_handler() use g_main_loop of default context.
For g_main_loop, refer to glib manual.</p>

@code
#include <stdio.h>
#include <glib.h>
#include <heynoti.h>

void callback(void *data)
{
	printf("I got a testnoti\n");
}

int main(int argc, const char *argv[])
{
	int fd;
	GMainLoop *loop;

	if((fd = heynoti_init()) < 0) {
		fprintf(stderr, "heynoti_init FAIL\n");
		return -1;
	}else
	printf("heynoti_init OK\n");

	if(heynoti_subscribe(fd, "test_testnoti", callback, NULL))
		fprintf(stderr, "heynoti_subscribe FAIL\n");
	else
		printf("heynoti_subscribe OK\n");

	if(heynoti_attach_handler(fd))
		fprintf(stderr, "heynoti_attach_handler FAIL\n");
	else
		printf("heynoti_attach_handler OK\n");

	loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(loop);

	heynoti_close(fd);
	g_main_loop_unref(loop);

	return 0;
}
@endcode

<p> And heynoti_poll_event() use polling mechanism. Therefore the process block until notification is received.</p>

@code
#include <stdio.h>
#include <heynoti.h>

void callback(void *data)
{
	printf("I got a testnoti\n");
}

int main(int argc, const char *argv[])
{
	int fd;

	if((fd = heynoti_init()) < 0) {
		fprintf(stderr, "heynoti_init FAIL\n");
		return -1;
	}else
		printf("heynoti_init OK\n");

	if(heynoti_subscribe(fd, "test_testnoti", callback, NULL))
		fprintf(stderr, "heynoti_subscribe FAIL\n");
	else
		printf("heynoti_subscribe OK\n");

	if(!heynoti_poll_event(fd))
		fprintf(stderr, "heynoti_poll_event FAIL\n");

	heynoti_unsubscribe(fd, "test_testnoti", callback);

	return 0;
}
@endcode

<p> If developer want to send notification message to the other process that watch the event, call heynoti_publish() API. </p>

@code
#include <stdio.h>
#include <heynoti.h>

int main(int argc, char** argv)
{
	if(argc != 2) {
		printf("[usage] %s <noti name>\n", argv[0]);
		return -1;
	}
	if(!heynoti_publish(argv[1])){
		fprintf(stderr, "heynoti_publish() FAIL\n");
		return -1;
	}

	return 0;
}
@endcode

<p> Also, heynoti can do the monitoring of file. This is a basic faculty of inotify. </p>

@code
#include <stdio.h>
#include <glib.h>
#include <heynoti.h>

void callback(void *data)
{
	printf("/home/test/noti_test_file was modified \n");
}

int main(int argc, const char *argv[])
{
	int fd;
	GMainLoop *loop;

	if((fd = heynoti_init()) < 0) {
		fprintf(stderr, "heynoti_init FAIL\n");
		return -1;
	}else
		printf("heynoti_init OK\n");

	if(heynoti_subscribe_file(fd, "/home/test/noti_test_file", callback, NULL, IN_CLOSE_WRITE | IN_DELETE_SELF))
		fprintf(stderr, "heynoti_subscribe_file FAIL\n");
	else
		printf("heynoti_subscribe_file OK\n");
	if(heynoti_attach_handler(fd))
		fprintf(stderr, "heynoti_attach_handler FAIL\n");
	else
		printf("heynoti_attach_handler OK\n");

	loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(loop);

	heynoti_close(fd);
	g_main_loop_unref(loop);

	return 0;
}
@endcode
*/

/**
@}
*/
