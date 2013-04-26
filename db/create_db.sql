-- install script for sinfonifry core

-- drop tables
drop table if exists sinf01_allowed_hosts;
drop table if exists sinf02_host_init;
drop table if exists sinf01_host_status;
drop table if exists sinf01_host;

-- clear the leftovers
drop database if exists sinfonifry;
drop role if exists sinfonifry;

-- Table type 01: table with permanent data
-- Table type 02: table with temporary data in it
-- Table type 03: table for plugin permanent data

-- create the requested sinfonifry user
create user sinfonifry createdb createuser password 's1nf0n1fry';

-- create a sinfonifry database
create database sinfonifry owner sinfonifry;

-- connect to the database
\connect sinfonifry

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
  CONSTRAINT "PK_host" PRIMARY KEY (host_id )
)
WITH (
  OIDS=FALSE
);

-- This table represents the last status of the host, as seen by various plugins. There should be
-- one entry per host/plugin, and the plugins should update always that entry. In the initialization
-- stage for the host, the core creates the entries for the plugins, and sets the show_status to 0
CREATE TABLE sinf01_host_status
(
  host_id bigserial NOT NULL,                    -- The key going towards the sinf01_host table
  plugin character varying(255),                 -- Which plugin has set this status
  host_last_status int,                          -- The status of the host: 0 - OK, 1 - Warning, 2 - Error, 3 - Host Down for more than 10 minutes. Implemented in core, used in WEB
  show_status int,                               -- Whether the status should be shown in the web gui or not. By default this is set to 0 (do not show)
  host_last_status_text character varying(255),  -- The explanation of the status of the host
  CONSTRAINT "PK_host_status" PRIMARY KEY (host_id, plugin),
  CONSTRAINT "FK_host" FOREIGN KEY (host_id)
        REFERENCES sinf01_host (host_id) MATCH SIMPLE
        ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (
  OIDS=FALSE
);

-- set the proper owner for the tables
ALTER TABLE sinf01_host OWNER TO sinfonifry;
ALTER TABLE sinf01_allowed_hosts OWNER TO sinfonifry;
ALTER TABLE sinf02_host_init OWNER TO sinfonifry;
ALTER TABLE sinf01_host_status OWNER TO sinfonifry;

-- here will go tables from the plugins
