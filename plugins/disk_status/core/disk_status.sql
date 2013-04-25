-- creates the tables that are required by the core plugin

-- this table represents the disk of a specific host.
CREATE TABLE sinf03_disk
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
);

-- this table holds the disk statistics that are sent regularly by the clients
CREATE TABLE sinf03_disk_statistics
(
   disk_stat_id bigserial NOT NULL,               -- The PK of this table
   disk_stat_disk_id bigserial NOT NULL,          -- Foreign key towards the "disk" table
   disk_stat_free_space bigserial,                -- the free space
   disk_stat_measurement_time timestamp,          -- when this was measured. Coming from the client in XML, seconds from Unix time 0
   CONSTRAINT pk_disk_stat PRIMARY KEY (disk_stat_id),
   CONSTRAINT fk_disk FOREIGN KEY (disk_stat_disk_id) REFERENCES sinf03_disk (disk_id) ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (
  OIDS = FALSE
);

-- and set the proper owner
ALTER TABLE sinf03_disk OWNER TO sinfonifry;
ALTER TABLE sinf03_disk_statistics OWNER TO sinfonifry;
