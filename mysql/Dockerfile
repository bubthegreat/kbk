# Use MySQL 8.3.0 - the last known working version
FROM mysql:8.3.0

# Copy the custom SQL script into the container
COPY init.sql /docker-entrypoint-initdb.d/
