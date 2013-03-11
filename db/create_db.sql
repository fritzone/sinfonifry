-- initial state, no user/db
drop database if exists sinfonifry;
drop role if exists sinfonifry;

-- create a sinfonifry user

create user sinfonifry createdb createuser password 's1nf0n1fry';

-- create a sinfonifry database
create database sinfonifry owner sinfonifry;

--
-- Table: sinf01_host - for storing all the host that are sending statistics
--
DROP TABLE if exists sinf01_host;

CREATE TABLE sinf01_host
(
  host_id bigserial NOT NULL, -- The primary key, and the host ID in the system
  host_ip inet NOT NULL, -- The IP of the host
  host_name character varying(255) NOT NULL, -- The name of the host
  CONSTRAINT "PK_host" PRIMARY KEY (host_id )
)
WITH (
  OIDS=FALSE
);
ALTER TABLE sinf01_host OWNER TO sinfonifry;

COMMENT ON TABLE sinf01_host IS 'This table represents a host.';
COMMENT ON COLUMN sinf01_host.host_id IS 'The primary key, and the host ID in the system';
COMMENT ON COLUMN sinf01_host.host_ip IS 'The IP of the host';
COMMENT ON COLUMN sinf01_host.host_name IS 'The name of the host';
