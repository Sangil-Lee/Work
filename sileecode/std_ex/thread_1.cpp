#include <cstdio>
#include <thread>

using namespace std;

void counter(int id, int length) {
	for(int i=1; i<=length; i++) {
		printf("counter[%d] : %d\n", id, i);
	}
}

int main() {
	thread t1(counter, 1, 5);
	thread t2(counter, 2, 7);
	t1.join();
	t2.join();

	return 0;
}
#if 0
위에서 join()은 각 Thread가 작업 완료될 때까지 Blocking되어 있도록 하는 명령어입니다. 
Blocking은 일반적으로 자원의 낭비를 가져오기 때문에 실제 프로그램에서는 join()의 사용을 최대한 피하는 것이 좋습니다. 
대신 Thread에 메세지(Message)를 처리하는 루틴을 만들고, Thread에 메세지를 보내어서 작업을 수행하는 방식이 좀 더 바람직합니다.
#endif
