/*
 * Copyright (c) 2016-20017 Max Cong <savagecm@qq.com>
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 #include <iostream>
 #include "logger/logger.h"
 #include "async/scheduler.h"
 #include <thread>
 
 using namespace std;
 
 void test_fun(int a, int b)
 {
	 std::cout << "a is " << a << " b is " << b << std::endl;
 }
 
 int main(int argc, char *argv[])
 {
	 IOService::Scheduler &scheduler = IOService::Scheduler::instance();
 
	 std::thread scheduler_thread([&scheduler] {
		 std::cout << "start a new thread to run boost io_service!" << std::endl;
		 scheduler.run();
		 std::cout << "should not run here" << std::endl;
	 });
 
	 IOService::invoke_now([] {
		 std::cout << "test function 1" << std::endl;
	 });
 
	 int test_a = 0;
	 int test_b = 1;
	 IOService::invoke_later([&] {
		 test_a = 10;
		 test_b = 20;
		 std::cout << "now a is " << test_a << " b is " << test_b << std::endl;
		 std::cout << "test function 2" << std::endl;
	 },
							 3000);
 
	 std::function<void()> fun = std::bind(&test_fun, test_a, test_b);
	 IOService::invoke_later(fun, 5000);
 
	 scheduler_thread.join();
 
	 return 0;
 }
 