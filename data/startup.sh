#!/usr/bin/env bash

mysql -u root -proot < configure_database.sql
cd /kbk/area && ./startup
