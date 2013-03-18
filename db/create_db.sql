-- clear the leftovers
drop database if exists sinfonifry;
drop role if exists sinfonifry;

-- Table type 01: table with permanent data
-- Table type 02: table with temporary data in it

-- create the requested sinfonifry user
create user sinfonifry createdb createuser password 's1nf0n1fry';

-- create a sinfonifry database
create database sinfonifry owner sinfonifry;

-- connect to the database
\connect sinfonifry

-- clear the possible leftovers. The order is important here
DROP TABLE if exists sinf01_disk_statistics;
DROP TABLE if exists sinf01_disk;
DROP TABLE if exists sinf01_host;
DROP TABLE if exists sinf01_allowed_hosts;
DROP TABLE if exists sinf02_host_init;

-- the hosts that are allowed to connect. When we add a host via the GUI it gets into this table
-- and the core component checks if the connection was made from a host which is in here
CREATE TABLE sinf01_allowed_hosts
(
   allowed_host_ip inet,                         -- The IP also acts as Primary Key
   allowed_host_name character varying(255),     -- The name of the host
   PRIMARY KEY (allowed_host_ip)
) 
WITH (
  OIDS = FALSE
)
;

-- the hosts that are allowed to connect and are in the initialization phase. 
-- When we add a host via the GUI it gets into this table and the core component 
-- checks if the connection was made from a host which is in here and in this case
-- it makes an extra step with the first XML sent: gathers all the disks and populates
-- the host disk configuration table
CREATE TABLE sinf02_host_init
(
   host_init_ip inet,                         -- The IP also acts as Primary Key
   PRIMARY KEY (host_init_ip)
) 
WITH (
  OIDS = FALSE
)
;

-- All the hosts that are present in the system. The web report is done based on this
CREATE TABLE sinf01_host
(
  host_id bigserial NOT NULL,                    -- The primary key
  host_ip inet NOT NULL,                         -- The IP of the host
  host_name character varying(255),              -- The name of the host
  host_last_seen_time timestamp,                 -- When we "saw" this host for the last time. The now() function of postgres
  host_last_status int,                          -- The status of the host: 0 - OK, 1 - Warning, 2 - Error, 3 - Host Down for more than 10 minutes. Implemented in core, used in WEB
  host_last_status_text character varying(255),  -- The explanation of the status of the host
  CONSTRAINT "PK_host" PRIMARY KEY (host_id )
)
WITH (
  OIDS=FALSE
);

-- this table represents the disk of a specific host.
CREATE TABLE sinf01_disk
(
   disk_id bigserial NOT NULL,                   -- The unique DISK id  
   host_id bigserial NOT NULL,                   -- Foreign key to the host table
   disk_fs_type character varying(255),          -- just to store the FS type
   disk_mountpoint character varying(255),       -- where the disk is mounted
   disk_physical_id character varying(255),      -- the /dev/sdXXX identifier
   disk_label character varying(255),            -- the label of the disk
   disk_total_space bigserial,                   -- how many free space in bytes the disk has
   CONSTRAINT pk_disk PRIMARY KEY (disk_id), 
   CONSTRAINT fk_host FOREIGN KEY (host_id) REFERENCES sinf01_host (host_id) ON UPDATE NO ACTION ON DELETE NO ACTION
) 
WITH (
  OIDS = FALSE
)
;

-- this table holds the disk statistics that are sent regularly by the clients
CREATE TABLE sinf01_disk_statistics
(
   disk_stat_id bigserial NOT NULL,               -- The PK of this table  
   disk_stat_disk_id bigserial NOT NULL,          -- Foreign key towards the "disk" table
   disk_stat_free_space bigserial,                -- the free space
   disk_stat_measurement_time timestamp,          -- when this was measured. Coming from the client in XML, seconds from Unix time 0
   CONSTRAINT pk_disk_stat PRIMARY KEY (disk_stat_id), 
   CONSTRAINT fk_disk FOREIGN KEY (disk_stat_disk_id) REFERENCES sinf01_disk (disk_id) ON UPDATE NO ACTION ON DELETE NO ACTION
) 
WITH (
  OIDS = FALSE
)
;

-- set the proper owner for the tables
ALTER TABLE sinf01_host OWNER TO sinfonifry;
ALTER TABLE sinf01_allowed_hosts OWNER TO sinfonifry;
ALTER TABLE sinf01_disk OWNER TO sinfonifry;
ALTER TABLE sinf02_host_init OWNER TO sinfonifry;

