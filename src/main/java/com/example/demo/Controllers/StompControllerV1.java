package com.example.demo.Controllers;


import lombok.extern.slf4j.Slf4j;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.messaging.handler.annotation.DestinationVariable;
import org.springframework.messaging.handler.annotation.MessageMapping;
import org.springframework.messaging.simp.SimpMessagingTemplate;
import org.springframework.web.bind.annotation.*;

@RestController
@Slf4j
@MessageMapping("/stompserver/api/v1") // must have simple broker perfix.
public class StompControllerV1 {

    Logger logger = LoggerFactory.getLogger(StompControllerV1.class);
    @Autowired
    private SimpMessagingTemplate messagingTemplate;


    @MessageMapping("/sendmessage/{to}")
    public void sendMessage(@DestinationVariable String to, String message) throws Exception {
        logger.info(message + " to " + to);
        messagingTemplate.convertAndSend("/stompserver/subscriber/" + to, message); //must have simple broker perfix.
    }

}