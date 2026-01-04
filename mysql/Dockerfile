# Use MySQL 8.4 LTS (Long Term Support) - latest LTS version
# MySQL 8.4 is the latest LTS and can upgrade from 8.3
FROM mysql:8.4

# Copy the custom SQL script into the container
COPY init.sql /docker-entrypoint-initdb.d/
