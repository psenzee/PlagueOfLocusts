
/* note vector db map onto MySQL
 
 CREATE TABLE types (
     id         INT UNSIGNED NOT NULL AUTO_INCREMENT,
     dimensions SMALLINT UNSIGNED NOT NULL DEFAULT 1,

     PRIMARY KEY (id)
 );

 CREATE TABLE types_dimensions (
     type_id       INT UNSIGNED NOT NULL DEFAULT 0,
     dimension     SMALLINT UNSIGNED NOT NULL DEFAULT 0,
     default_value DOUBLE NOT NULL DEFAULT 0.0,

     minimum       DOUBLE DEFAULT NULL,
     maximum       DOUBLE DEFAULT NULL,
     mean          DOUBLE DEFAULT NULL,
     median        DOUBLE DEFAULT NULL,
     variance      DOUBLE DEFAULT NULL,

     PRIMARY KEY (type_id, dimension),
     INDEX (type_id, minimum),
     INDEX (type_id, maximum),
     INDEX (type_id, mean),
     INDEX (type_id, median),
     INDEX (type_id, variance)
 );
 
 CREATE TABLE vectors (
     id         INT UNSIGNED NOT NULL AUTO_INCREMENT,
     type_id    INT UNSIGNED NOT NULL DEFAULT 0,

     PRIMARY KEY (id),
     INDEX (type_id, id)
 );
 
 CREATE TABLE vectors_tags (
     vector_id  INT UNSIGNED NOT NULL DEFAULT 0,
     tag_id     INT UNSIGNED NOT NULL DEFAULT 0,

     PRIMARY KEY (vector_id, tag_id),
     INDEX (tag_id)
 );
 
 CREATE TABLE vectors_dimensions_tags (
     vector_id  INT UNSIGNED NOT NULL DEFAULT 0,
     dimension  SMALLINT UNSIGNED NOT NULL DEFAULT 0,
     tag_id     INT UNSIGNED NOT NULL DEFAULT 0,

     PRIMARY KEY (vector_id, dimension, tag_id),
     INDEX (tag_id, vector_id),
     INDEX (dimension, tag_id)
 );
 
 CREATE TABLE vectors_magnitudes (
     vector_id INT UNSIGNED NOT NULL DEFAULT 0,
     magnitude DOUBLE NOT NULL DEFAULT 1.0,
 
     PRIMARY KEY (vector_id),
     INDEX (vector_id, magnitude),
     INDEX (magnitude)
 );
 
 CREATE TABLE vectors_values_sparse (
     vector_id INT UNSIGNED NOT NULL DEFAULT 0,
     dimension SMALLINT UNSIGNED NOT NULL DEFAULT 0,
     value     DOUBLE NOT NULL DEFAULT 0.0,
 
     PRIMARY KEY (vector_id, dimension, value),
     INDEX (vector_id, dimension),
     INDEX (dimension, value)
 );
 
 ... FROM vectors_magnitudes m INNER JOIN vectors_values_sparse v ON m.vector_id=v.vector_id
 
 -- very unwieldy
 CREATE TABLE vectors_values_dense4 (

     vector_id    INT UNSIGNED NOT NULL DEFAULT 0,
 
     d0  SMALLINT UNSIGNED NOT NULL DEFAULT 0,
     d1  SMALLINT UNSIGNED NOT NULL DEFAULT 0,
     d2  SMALLINT UNSIGNED NOT NULL DEFAULT 0,
     d3  SMALLINT UNSIGNED NOT NULL DEFAULT 0,

     v0  DOUBLE NOT NULL DEFAULT 0.0,
     v1  DOUBLE NOT NULL DEFAULT 0.0,
     v2  DOUBLE NOT NULL DEFAULT 0.0,
     v3  DOUBLE NOT NULL DEFAULT 0.0,
 
     PRIMARY KEY (vector_id, d0, d1, d2, d3),
     INDEX (d0, d1, d2, d3,  v0, v1, v2, v3),
     INDEX (d1, d2, d3,      v1, v2, v3),
     INDEX (d0, d2, d3,      v0, v2, v3),
     INDEX (d0, d1, d3,      v0, v1, v3),
     INDEX (d0, d3,          v0, v3),
     INDEX (d1, d3,          v1, v3),
     INDEX (d2, d3,          v2, v3),
     INDEX (d3,              v3)
 );

 */
