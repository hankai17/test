#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <syslog.h>
#include <pthread.h>

typedef unsigned long long uint64_t;

const unsigned char BIT_HEX[] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};

#define _isset(bt,idx)		(bt->b[(idx) / 8 ] & BIT_HEX[(idx) % 8])
#define _isempty(bt) 		(bt->nset == 0)
#define _isfull(bt) 			(bt->nset >= bt->nbits)

#define MAX_FILE_PATH_LEN (256)
#define RANGE_MIN_ZONE_SIZE (20)

typedef struct {
	uint64_t start;
	uint64_t end;
} ZONE;

typedef struct _BitField {
	char bf_file[MAX_FILE_PATH_LEN];
	int nbits;
	int nbytes;
	int nwrite_set;
	unsigned char* b;
	unsigned char* rb;
	int nset;
	int64_t file_len;
	int zone_max_size;
	int zone_size;
	pthread_mutex_t	bitmap_mutex;
} BitField;

int bt_Init( BitField* bt , int npcs );
int bt_recall( BitField* bt , int npcs);
void bt_fini( BitField* bf );
void bt_recalc( BitField* bt );
int bt_percent( BitField* bt );
void bt_set( BitField* bt ,int idx);

void bt_SetAll( BitField* bt );
void bt_Set( BitField* bt , int idx);

int bt_IsSet( BitField* bt , int idx);
int bt_IsFull( BitField *bt );
int bt_IsEmpty( BitField *bt );

int bt_WriteToFile( BitField* bt , const char *fname);
int bt_SetReferFile( BitField* bt , const char *fname);
void bt_RangeSet(BitField* bf,uint64_t in_start,uint64_t size);
void bt_qsort_zone(BitField* bf);
BitField* bt_create_range(const char* bt_filename, uint64_t file_len);

int bt_Init( BitField* bt , int npcs) {
	//bt->b = NULL;
	//bt->rb = NULL;
	memset(bt->bf_file,0x0,MAX_FILE_PATH_LEN);
	bt->nbits = npcs;
	bt->nbytes = bt->nbits / 8;
	if( bt->nbits % 8 ) bt->nbytes++;
	bt->b =  (unsigned char*)malloc( bt->nbytes );
	if( !bt->b ) return -1;
	memset(bt->b, 0, bt->nbytes);
	bt->nset = 0;
	bt->nwrite_set = 0;
	return 0;
}

int bt_recall( BitField* bt, int npcs) {
	memset(bt->bf_file,0x0,MAX_FILE_PATH_LEN);
	bt->nbits = npcs;
	bt->nbytes = bt->nbits / 8;
	if( bt->nbits % 8 ) bt->nbytes++;
	bt->b =  (unsigned char*)realloc( bt->b , bt->nbytes );
	if( !bt->b ) return -1;
	memset(bt->b, 0, bt->nbytes);
	bt->nset = 0;
	bt->nwrite_set = 0;
	return 0;
}

void bt_fini( BitField* bf) {
	if(bf == NULL)	 return ;
	if(bf->b) {
		free(bf->b);
		bf->b = NULL;
	}
	if(bf->rb) {
		free(bf->rb);
		bf->rb = NULL;
	}
}

void bt_SetAll(BitField* bt) {
	int i;
	if( NULL == bt || NULL == bt->b || 0 >= bt->nbytes ) return;
	bt->nset = bt->nbits;
	bt->nwrite_set = bt->nbits;
	memset(bt->b,0xFF,bt->nbytes - 1);
	if( bt->nbits % bt->nbytes ) {
		for(i = 8 * (bt->nbytes - 1); i < bt->nbits; i++)
		bt->b[i / 8] |= BIT_HEX[i % 8];
	} else
		bt->b[bt->nbytes - 1] = (unsigned char) 0xFF;
}

int bt_IsSet( BitField* bt, int idx) {
	if( NULL == bt || NULL == bt->b ) return 0;
	if( idx >= bt->nbits ) return 0;
	return _isfull(bt) ? 1 : _isset(bt,idx);
}

void bt_Set( BitField* bt , int idx)
{
	if( NULL == bt || NULL == bt->b ) return;
	
	if(idx >= bt->nbits) return;
	//printf("bt_Set %d\n",idx);
	if( !_isfull(bt) && !_isset(bt,idx) ){
		bt->b[idx / 8] |= BIT_HEX[idx % 8];
		bt->nset++;
	}
}

int bt_IsFull( BitField *bt ) {
	if( NULL == bt || NULL == bt->b || bt->nbits == 0) return 0;
	return (bt->nset >= bt->nbits) ? 1 : 0; 
}

int bt_IsEmpty( BitField *bt ) {
	if( NULL == bt || NULL == bt->b ) return 1;
	return (bt->nset == 0) ? 1 : 0; 
}

void bt_set( BitField* bt , int idx) {
	if( idx < bt->nbits && !_isfull(bt) && !_isset(bt,idx) )
		bt->b[idx / 8] |= BIT_HEX[idx % 8];
}

void bt_recalc(BitField* bt) {
	int i;
	for(bt->nset = 0, i = 0; i < bt->nbits; i++) 
		if( _isset(bt,i)) bt->nset++;
}

int bt_percent( BitField* bt) {
	if(0 == bt->nbits) return -1;
	return (bt->nset*100) / bt->nbits;
}

int bt_SetReferFile(BitField* bt, const char* fname) {
	FILE* fp;
	struct stat sb;
	if(stat(fname, &sb) < 0) return -1;
	if(sb.st_size != bt->nbytes) return -1;

	bt->nbytes = sb.st_size;
	fp = fopen(fname, "r");
	if( !fp ) return -1;
	if( fread(bt->b, sb.st_size, 1, fp) != 1 ) {
		fclose(fp);
		return -1;
	}
	fclose(fp);
	bt_recalc( bt );
	bt->nwrite_set = bt->nset;
	return 0;
}

int bt_WriteToFile(BitField* bt , const char* fname) {
	FILE* fp = NULL;
	if( NULL == bt || NULL == bt->b || 0 >= bt->nbytes ) return -1;
	if(bt->nwrite_set == bt->nset) return 0;

	fp = fopen(fname, "w");
	if( !fp ) return -1;

	if( fwrite(bt->b, bt->nbytes, 1, fp) != 1 ) {
		fclose(fp);
		return -1;
	}
	fclose(fp);
	bt->nwrite_set = bt->nset;
	return 0;
}

int bt_is_full( BitField *bf) {
	if( NULL == bf || NULL == bf->rb ) return 0;
	ZONE* zone = (ZONE *)bf->rb ;

	if(zone->start == 0 && zone->end == bf->file_len - 1)
		return 1;
	else
		return 0;
}

void bt_set_full(BitField* bf) {
	if( NULL == bf || NULL == bf->rb ) return;
	ZONE * z = (ZONE *)bf->rb;
	bf->zone_size = 1;
	z->start=0;
	z->end = bf->file_len - 1;
}

void bt_clear( BitField* bf ) {
	if( NULL == bf ) return;
	if( NULL != bf->b && bf->nbytes > 0) {
		memset( bf->b , 0x0 , bf->nbytes );
		bf->nset = 0;
		bf->nwrite_set = 0;
	}

	if( NULL != bf->rb && bf->zone_max_size > 0) {
		memset( bf->rb , 0x0 , bf->zone_max_size*sizeof(ZONE) );
		bf->zone_size = 0;
	}
}

BitField* bt_create_range(const char* bt_filename, uint64_t file_len) {
	struct stat buf;
	if(0 > stat(bt_filename, &buf) || 0 == buf.st_size ) { //a new file
		//BitField*	 bf =(BitField*) calloc(sizeof(BitField),1);
		BitField*	 bf = (BitField*)malloc(sizeof(BitField) );
		if(bf != NULL) {
			pthread_mutexattr_t mutexattr;
			pthread_mutexattr_init(&mutexattr);
			pthread_mutexattr_settype(&mutexattr,PTHREAD_MUTEX_DEFAULT);	
			pthread_mutex_init(&(bf->bitmap_mutex),&mutexattr);
			pthread_mutexattr_destroy(&mutexattr);	
			strncpy(bf->bf_file, bt_filename, MAX_FILE_PATH_LEN - 1);
		}

		bf->rb = calloc(RANGE_MIN_ZONE_SIZE * sizeof(ZONE), 1);
		bf->zone_max_size = RANGE_MIN_ZONE_SIZE;
		bf->zone_size = 0;
		bf->file_len = file_len;
		return bf;
	}

	BitField*	 bf = malloc(sizeof(BitField) );
	if(bf != NULL) {
		pthread_mutexattr_t mutexattr;
		pthread_mutexattr_init(&mutexattr);
		pthread_mutexattr_settype(&mutexattr,PTHREAD_MUTEX_DEFAULT);	
		pthread_mutex_init(&(bf->bitmap_mutex),&mutexattr);
		pthread_mutexattr_destroy(&mutexattr);	
		strncpy(bf->bf_file, bt_filename, MAX_FILE_PATH_LEN - 1);
	}

	if(buf.st_size < RANGE_MIN_ZONE_SIZE * sizeof(ZONE)) { //normal
		bf->rb = calloc(RANGE_MIN_ZONE_SIZE * sizeof(ZONE), 1);
		bf->zone_max_size = RANGE_MIN_ZONE_SIZE;
		bf->zone_size = buf.st_size / sizeof(ZONE); //文件里只存zone
	} else {
		bf->rb = malloc(buf.st_size);
		memset(bf->rb, 0x0, buf.st_size);
		bf->zone_max_size = buf.st_size / sizeof(ZONE);
		bf->zone_size = buf.st_size / sizeof(ZONE);
	}

	bf->file_len = file_len;	
	FILE* fp = fopen(bt_filename, "rb"); //打开文件 开始读
	if( NULL == fp ) {
		free(bf->rb);
		free(bf);
		return NULL;
	}
	
	if(1 != fread(bf->rb,buf.st_size,1,fp)) {
		fclose(fp);	
		free(bf->rb);
		free(bf);
		return NULL;
	}

	fclose(fp);
	return bf;
}

int bt_recall_range(BitField* bf, const char* bt_filename, uint64_t file_len) {
	if(!bf || !bt_filename) return -1;
	memset(bf->bf_file, 0x0, MAX_FILE_PATH_LEN);
	strncpy(bf->bf_file, bt_filename, MAX_FILE_PATH_LEN - 1);
	if(!bf->rb)
		bf->rb = calloc(RANGE_MIN_ZONE_SIZE * sizeof(ZONE),1);
	else
		bf->rb = (unsigned char * )realloc(bf->rb, RANGE_MIN_ZONE_SIZE * sizeof(ZONE));
	bf->zone_max_size = RANGE_MIN_ZONE_SIZE;
	bf->zone_size = 0;
	bf->file_len=file_len;
	return 0;
}

int bt_reset_range(BitField* bf , uint64_t file_len) {
	if( NULL == bf ) return -1;
	if( NULL == bf->rb)
		bf->rb = calloc(RANGE_MIN_ZONE_SIZE* sizeof(ZONE),1);
	else {
		if( bf->zone_max_size > 0 )
			memset( bf->rb , 0x0 , bf->zone_max_size*sizeof(ZONE) );
		else
			memset( bf->rb , 0x0 , RANGE_MIN_ZONE_SIZE*sizeof(ZONE) );
	}
	 
	bf->zone_max_size = RANGE_MIN_ZONE_SIZE;
	bf->zone_size = 0;
	bf->file_len = file_len;
	return 0;
}

int bt_RangeIsSet(BitField* bf, uint64_t start, uint64_t end) { //是否在某个zone里
	int i = 0;
	ZONE* zone;
	if(bf == NULL || bf->rb == NULL) return 0;
	for(i = 0; i < bf->zone_size; ++i) {
		zone = (ZONE *)(bf->rb + i * sizeof(ZONE));
		if(start >= zone->start  && end <= zone->end) return 1;
	}
	return 0;
}

void bt_release_range(BitField* bf) {
	if(bf == NULL)	 return ;
	if(bf->b) {
		free(bf->b);
		bf->b = NULL;
	}
	if(bf->rb) {
		free(bf->rb);
		bf->rb = NULL;
	}
	free(bf);
}

int merge_from_file(BitField* bf,const char* file) { //文件中的start-end对组转到bt结构体中
	struct stat file_stat;
	ZONE* zone;
	int i;
	unsigned char* buf ;
	FILE* fp ;
	
	if(0 > stat(file, &file_stat)) return -1;
	if(NULL == (fp = fopen(file,"rb"))) return -1;
	buf = (unsigned char * )malloc(file_stat.st_size);
	if(1 != fread(buf, file_stat.st_size, 1, fp)) {
		fclose(fp);
		free(buf);
		return -1;
	}
	fclose(fp);

	for(i = 0; i < file_stat.st_size/sizeof(ZONE); ++i) {
		zone = (ZONE *)(buf + i*(sizeof(ZONE)));
		bt_RangeSet(bf, zone->start, zone->end - zone->start + 1); //从文件中读到buf里 转成zone 设到bt里 //这个buf还不能复用...
	}

	free(buf);
	return 0;
}

void print_bt_dump(BitField* bf) {
	int i=0;
	ZONE* zone;
	if(bf == NULL || bf->zone_size == 0 || bf->rb == NULL) return ;
	for(i = 0; i < bf->zone_size; ++i) {
		zone = (ZONE *)(bf->rb + i * sizeof(ZONE));
		printf("print_bt_dump bf(%p):(%03d):(%ld)-(%ld)",bf,i,zone->start,zone->end);
	}
}

void bt_Lock(BitField* bf) {
	pthread_mutex_lock(&(bf->bitmap_mutex));	
}

void bt_Unlock(BitField* bf) {
	pthread_mutex_unlock(&(bf->bitmap_mutex));	
}

void bt_RangeSet(BitField* bf, uint64_t in_start, uint64_t size) {
	int i = 0;
	ZONE* zone;
	uint64_t in_end = in_start + size - 1;
	int flag = 0;
	uint64_t dst_start = 0, dst_end = 0;	

	if(bf  == NULL || bf->zone_max_size== 0 || bf->rb == NULL || bt_is_full(bf)) return ;
	if(in_start > bf->file_len || size > bf->file_len || in_start + size > bf->file_len) return;
	printf("in_start(%llu)-end(%llu)",in_start,in_end);

	if(bf->zone_size == 0) { //插入第一个位置
		zone = (ZONE *)(bf->rb);
		zone->start = in_start;
		zone->end = in_end;
		bf->zone_size++;
		goto END;
	}

	for(i = 0; i < bf->zone_size; ++i) {
		flag = 0;
		zone = (ZONE*)(bf->rb + i * sizeof(ZONE));
		dst_start = 0; dst_end = 0;

		if(bt_merge_zone(&dst_start, &dst_end, zone->start, zone->end, in_start, in_end) == 0) { //can merge 见缝插针 merge也是跟前一个zone merge
			zone->start = dst_start;
			zone->end = dst_end;
			goto END;
		}

		if(in_end < zone->start) {
			if(bf->zone_size < bf->zone_max_size) {
				flag = 1;
				memmove(zone + 1, zone, (bf->zone_size - i) * sizeof(ZONE)); //zone后移
				zone->start = in_start;
				zone->end = in_end;
				bf->zone_size += 1;
			} else {
				flag=2;
				bf->rb = (unsigned char * )realloc(bf->rb,bf->zone_max_size *sizeof(ZONE)*2);
				memset(bf->rb + bf->zone_max_size *sizeof(ZONE),0x00,bf->zone_max_size *sizeof(ZONE));
				bf->zone_max_size *= 2;

				zone = (ZONE *)(bf->rb + i * sizeof(ZONE));
				memmove(zone+1,zone,(bf->zone_size - i) * sizeof(ZONE));
				zone->start = in_start;
				zone->end = in_end;
				bf->zone_size	+= 1;
			}
			goto END;
		}
	}

	if(bf->zone_size < bf->zone_max_size) { 
	} else {
		//unsigned char *tmp =  (unsigned char * )calloc(2,bf->zone_max_size *sizeof(ZONE));
		//memcpy(tmp,bf->rb,bf->zone_max_size *sizeof(ZONE));
		//free(bf->rb);
		//bf->rb = tmp;
		bf->rb = (unsigned char * )realloc(bf->rb,bf->zone_max_size *sizeof(ZONE)*2);
		memset(bf->rb + bf->zone_max_size *sizeof(ZONE),0x00,bf->zone_max_size *sizeof(ZONE));

		bf->zone_max_size = bf->zone_max_size *2;
	}
	zone = (ZONE *)(bf->rb + i * sizeof(ZONE));
	zone->start = in_start;
	zone->end = in_end;
	bf->zone_size += 1;
END:
	return;
}

void bt_dump(BitField* bf) {
	if(bf  == NULL || bf->zone_size == 0 || bf->rb == NULL) return ;
#if 1
	merge_from_file(bf, bf->bf_file);
	bt_qsort_zone(bf);
	FILE* fp = fopen(bf->bf_file,"wb+");
	if( NULL == fp ) return ;
	fwrite(bf->rb, bf->zone_size * sizeof(ZONE), 1, fp); //只写zones
	fclose(fp);
#endif	
	print_bt_dump(bf);
}

// ret == 0 merge ok   < 0 can not merge
int bt_merge_zone(uint64_t* dst_start, uint64_t* dst_end, uint64_t start1, uint64_t end1, uint64_t start2, uint64_t end2) {
	uint64_t len1 = end1 - start1;
	uint64_t len2 = end2 - start2;

	if(len1 >= len2) {
		if(start2 < start1 && end2 < start1 -1) return -1;

		if(start2 < start1 && end2 ==  start1 -1) {
			*dst_start = start2; *dst_end = end1;
			return 0;
		}

		if(start2 <=start1 && end2 <= end1) {
			*dst_start = start2; *dst_end = end1;
			return 0;
		}

		if(start2 >= start1 && end2 <= end1) {
			*dst_start = start1; *dst_end = end1;
			return 0;
		}

		if(start2 >= start1 &&  start2 < end1 &&end2 > end1) {
			*dst_start = start1; *dst_end = end2;
			return 0;
		}

		if(start2 == end1 + 1) {
			*dst_start = start1; *dst_end = end2;
			return 0;
		}

		if(start2 < end1 + 1 && start2 > start1) {
			if(end2 > end1) {
				*dst_start = start1; *dst_end = end2;
			} else {
				*dst_start = start1; *dst_end = end1;
			}
			return 0;
		}
		return -1;
	} else {
		return bt_merge_zone(dst_start, dst_end, start2, end2, start1, end1);
	}

}

void bt_qsort_zone(BitField* bf) {
	int i=0;
	ZONE* first_zone, *last_zone, *zone1, *zone2;
	uint64_t dst_start, dst_end;
	int do_againt = 1;
	
	if(bf  == NULL || bf->zone_size == 0 || bf->rb == NULL) return ;
	
	while(bf->zone_size > 1 && do_againt) {
		do_againt =0;

		first_zone = (ZONE *)bf->rb;
		last_zone = first_zone + bf->zone_size -1;

		for(i = 1; i < bf->zone_size; ++i) {
			zone2 = first_zone + i;
			zone1 = zone2 - 1;
			dst_start = dst_end = 0;

			if(0 == bt_merge_zone(&dst_start, &dst_end, zone1->start, zone1->end, zone2->start, zone2->end)) {
				zone1->start = dst_start;
				zone1->end = dst_end;

				if(i == bf->zone_size - 1) {
					zone2->start = zone2->end = 0;
				} else {
					memmove(zone2, zone2 + 1, (bf->zone_size - i -1) * sizeof(ZONE)); //cover it
					last_zone->start = last_zone->end = 0;
				}
				bf->zone_size --;
				do_againt = 1;
				break;
			}
		}
	}
}

int main() {
	uint64_t zones[][2]=
	{
		{20,30},
		{15,25},
		{26,30},		
		{26,31},
		{40,50},
		{45,60},
		{70,80}
	};
	int i;
	ZONE* zone; 

	BitField* bf = bt_create_range("/tmp/aaaa",0xfffffff);
	ZONE* first_zone = (ZONE *)bf->rb;

	for(i=0;i< sizeof(zones)/sizeof(ZONE);i++)
	{
		(first_zone+i)->start = zones[i][0];
		(first_zone+i)->end = zones[i][1];		
	}
	bf->zone_size = sizeof(zones)/sizeof(ZONE);

	bt_qsort_zone(bf);
	bt_dump(bf);
	//print_bt_dump(bf);

	for(i=0;i< bf->zone_size;++i) {
		zone = (ZONE *)(bf->rb + i * sizeof(ZONE));
		printf("zone i: %d, start: %d, end %d\n", i, zone->start, zone->end);
	}

/*
zone i: 0, start: 15, end 31
zone i: 1, start: 40, end 60
zone i: 2, start: 70, end 80
*/

	bt_release_range(bf);
	return 0;
}

