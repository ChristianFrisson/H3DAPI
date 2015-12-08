-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               10.1.9-MariaDB - mariadb.org binary distribution
-- Server OS:                    Win64
-- HeidiSQL Version:             9.1.0.4867
-- --------------------------------------------------------


CREATE DATABASE IF NOT EXISTS `testserver`;
USE `testserver`;


CREATE TABLE IF NOT EXISTS `performance_results` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `test_run_id` int(10) unsigned NOT NULL,
  `file_id` int(10) unsigned NOT NULL,
  `case_id` int(10) unsigned NOT NULL,
  `min_fps` float unsigned NOT NULL,
  `max_fps` float unsigned NOT NULL,
  `avg_fps` float unsigned NOT NULL,
  `mean_fps` float unsigned NOT NULL,
  `full_case_data` text NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;



CREATE TABLE IF NOT EXISTS `screenshot_baselines` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `test_run_id` int(10) unsigned NOT NULL,
  `file_id` int(10) unsigned NOT NULL,
  `case_id` int(10) unsigned NOT NULL,
  `image` mediumblob NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `screenshot_results` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `test_run_id` int(10) unsigned NOT NULL,
  `file_id` int(10) unsigned NOT NULL,
  `case_id` int(10) unsigned NOT NULL,
  `output_image` mediumblob NOT NULL,
  `success` enum('Y','N') NOT NULL,
  `diff_image` mediumblob NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

CREATE TABLE IF NOT EXISTS `servers` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `server_name` tinytext NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `test_cases` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `case_name` text NOT NULL,
  `test_type` enum('performance','rendering') NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `test_files` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `filename` text NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `test_runs` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `timestamp` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `server_id` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;