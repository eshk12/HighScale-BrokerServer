package com.example.demo.Models;

public class CommandEntity {
    private String deviceSerial;
    private String command;

    public CommandEntity(String deviceSerial, String command) {
        this.deviceSerial = deviceSerial;
        this.command = command;
    }

    public String getDeviceSerial() {
        return deviceSerial;
    }

    public void setDeviceSerial(String deviceSerial) {
        this.deviceSerial = deviceSerial;
    }

    public String getCommand() {
        return command;
    }

    public void setCommand(String command) {
        this.command = command;
    }
}
