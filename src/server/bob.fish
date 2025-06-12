#!/usr/bin/env fish
set addr "localhost:18080"
set uname bob

function GET
    curl --silent "$addr/$argv[1]" | jq
    echo
end

function POST
    curl --silent -XPOST "$addr/$argv[1]"
    echo
end

GET "join?username=$uname"

POST "bet/$uname?amount=400"

GET game_state

read -P 'action: ' action
POST "move/$uname?action=$action"
GET game_state
