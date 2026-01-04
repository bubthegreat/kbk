# Use an official MySQL image as the base image
FROM mysql:latest

# Copy the custom SQL script into the container
COPY init.sql /docker-entrypoint-initdb.d/
