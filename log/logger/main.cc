#include"logger.hh"  
#include<syslog.h>
#include<pthread.h>
#define L theL()  //默认返回 log_id为空 设备为daemon的log对象
//#define L1 theL()

  //enum Urgency {All=32767,Alert=LOG_ALERT 1, Critical=LOG_CRIT 2, Error=LOG_ERR 3, Warning=LOG_WARNING 4,
  //Notice=LOG_NOTICE 5,Info=LOG_INFO 6, Debug=LOG_DEBUG 7, None=-1};
int main()
{
   L.setName("L"); //里面调用open
   L.setLoglevel((Logger::Urgency)(7));
   //L.setLoglevel((Logger::Urgency::Info)); //icdns.log文件中打印比urgency低的所有信息
   L.setFacility(LOG_LOCAL0); //里面调用open
    /*
   L1.setName("L1");
   L1.setFacility(LOG_LOCAL1);
   */

   L<<Logger::Alert<<"alert test "<<std::endl;
   L<<Logger::Critical<<"critical test "<<std::endl;
   L<<Logger::Error<<"err test "<<std::endl;
   L<<Logger::Warning<<"warn test "<<std::endl;
   L<<Logger::Notice<<"notice test "<<std::endl;
   L<<Logger::Info<<"info test "<<std::endl;
   L<<Logger::Debug<<"debug test "<<std::endl;
    /*
   L1<<Logger::Alert<<"alert test "<<std::endl;
   L1<<Logger::Critical<<"critical test "<<std::endl;
   L1<<Logger::Error<<"err test "<<std::endl;
   L1<<Logger::Warning<<"warn test "<<std::endl;
   L1<<Logger::Notice<<"notice test "<<std::endl;
   L1<<Logger::Info<<"info test "<<std::endl;
   L1<<Logger::Debug<<"debug test "<<std::endl;
   */
}

/*
void foo(void* v)
{
   int i = 0;
   pthread_t id = pthread_self();
   for(i = 0; i < 26; i++ ) {
    L<<Logger::Alert<<id<<"---alert test "<<i<<std::endl;
   }
   //L<<Logger::Alert<<id<<"---alert test "<<std::endl;
   //L<<Logger::Critical<<id<<"critical test "<<std::endl;
   //L<<Logger::Error<<id<<"err test "<<std::endl;
   //L<<Logger::Warning<<id<<"warn test "<<std::endl;
   //L<<Logger::Notice<<id<<"notice test "<<std::endl;
   //L<<Logger::Info<<id<<"info test "<<std::endl;
   //L<<Logger::Debug<<id<<"debug test "<<std::endl;
}

int main()
{
    pthread_t pid[5];
    int i;

    L.setName("L");
    L.setFacility(LOG_LOCAL0);

    for(i = 0; i < 5; i++) {
        pthread_create(pid + i, NULL, foo, NULL);
    }

    for(i = 0; i < 5; i++) {
        pthread_join(pid[i], NULL);
    }
    return 0;
}
*/

