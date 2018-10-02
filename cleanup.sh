
# Stop the dockers
docker stop kbk_service_container
docker stop kbk_sql_container
echo "KBK Containers stopped."

docker rm kbk_service_container
docker rm kbk_sql_container
echo "KBK Containers removed."
