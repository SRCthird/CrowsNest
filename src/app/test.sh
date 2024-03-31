#!/bin/bash

# Base URL of your Crow application
BASE_URL="http://localhost:18080"

# Test the home page
echo "Testing home page:"
curl "$BASE_URL/"
echo -e "\n"

# Test creating a single user
echo "Testing creating a single user:"
curl -X POST -H "Content-Type: application/json" -d '{"id": 1, "name": "John Doe"}' "$BASE_URL/create"
echo -e "\n"

# Test creating multiple users
echo "Testing creating multiple users:"
curl -X POST -H "Content-Type: application/json" -d '[{"id": 2, "name": "Jane Doe"}, {"id": 3, "name": "Jim Doe"}]' "$BASE_URL/create_bulk"
echo -e "\n"

# Test getting a single user
echo "Testing getting a single user:"
curl "$BASE_URL/get/1"
echo -e "\n"

# Test getting all users
echo "Testing getting all users:"
curl "$BASE_URL/get"
echo -e "\n"

# Test getting users by ids
echo "Testing getting users by ids:"
curl "$BASE_URL/get?id=2,3"
echo -e "\n"

# Test updating a user
echo "Testing updating a user:"
curl -X PUT -H "Content-Type: application/json" -d '{"id": 1, "name": "John Updated"}' "$BASE_URL/update"
echo -e "\n"

# Test deleting a single user
echo "Testing deleting a single user:"
curl -X DELETE "$BASE_URL/delete/1"
echo -e "\n"

# Test deleting multiple users
echo "Testing deleting multiple users:"
curl -X DELETE "$BASE_URL/delete?id=2,3"
echo -e "\n"

