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

-- clear the possible leftovers
DROP TABLE if exists sinf01_host;
DROP TABLE if exists sinf01_allowed_hosts;
DROP TABLE sinf02_host_init;

-- the hosts that are allowed to connect. When we add a host via the GUI it gets into this table
-- and the core component checks if the connection was made from a host which is in here
CREATE TABLE sinf01_allowed_hosts
(
   allowed_host_ip inet,                         -- The IP also acts as Primary Key
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
  host_name character varying(255) NOT NULL,     -- The name of the host
  host_last_seen_time date,                      -- When we "saw" this host
  CONSTRAINT "PK_host" PRIMARY KEY (host_id )
)
WITH (
  OIDS=FALSE
);

-- this table represents the disk of a specific host.
CREATE TABLE sinf01_disk
(
   disk_id bigserial,                            -- The unique DISK id  
   host_id bigserial,                            -- Foreign key to the host table
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

-- set the proper owner for the tables
ALTER TABLE sinf01_host OWNER TO sinfonifry;
ALTER TABLE sinf01_allowed_hosts OWNER TO sinfonifry;
ALTER TABLE sinf01_disk OWNER TO sinfonifry;
ALTER TABLE sinf02_host_init OWNER TO sinfonifry;

