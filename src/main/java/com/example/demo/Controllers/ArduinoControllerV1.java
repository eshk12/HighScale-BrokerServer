package com.example.demo.Controllers;


import com.example.demo.Models.CommandEntity;
import com.example.demo.Models.Greeting;
import com.example.demo.Models.WebSocketResponseEntity;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.messaging.handler.annotation.MessageMapping;
import org.springframework.messaging.handler.annotation.SendTo;
import org.springframework.messaging.simp.SimpMessagingTemplate;
import org.springframework.web.bind.annotation.*;

@RestController

@MessageMapping("/arduino/api/v1")
@RequestMapping("/arduino/api/v1")
public class ArduinoControllerV1 {

    @Autowired
    private SimpMessagingTemplate template;

    @MessageMapping("/getArduinoResponse")
    @SendTo("/arduino/subscribers")
    public WebSocketResponseEntity getArduinoResponse(WebSocketResponseEntity message) throws Exception {
        return message;
    }


    @PostMapping("/sendCommand")
    public String sendCommand(@RequestBody CommandEntity commandEntity) throws Exception {
        this.template.convertAndSend("/arduino/subscribers", commandEntity.getCommand());
        return String.format("Sending to %s the command: %s ", commandEntity.getDeviceSerial(), commandEntity.getCommand());
    }
}