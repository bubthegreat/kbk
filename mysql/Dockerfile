# Use MySQL 5.7 for compatibility with old authentication
FROM mysql:5.7

# Copy the custom SQL script into the container
COPY init.sql /docker-entrypoint-initdb.d/
