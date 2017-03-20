create database mydb_utf1 character set utf8 ; 

use mydb_utf1;

DROP TABLE IF EXISTS `ip`;
CREATE TABLE `ip`(
	`id`     int(4) PRIMARY KEY,
 	`ip`       varchar(15) NOT NULL,
 	`detail`   varchar(64)
)DEFAULT CHARSET=utf8;

INSERT INTO ip VALUES(1000, '255.255.255.255', 'NEW YORK');
INSERT INTO ip VALUES(1001, '255.255.255.255', 'DALLAS');
INSERT INTO ip VALUES(1002, '255.255.255.255', 'CHICAGO');
INSERT INTO ip VALUES(1003, '0.0.0.0', 'BOSTON');

commit;
    
