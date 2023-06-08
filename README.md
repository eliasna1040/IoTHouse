# IoTHouse

## Table of Contents
[About the project](#about-the-project)

[Requirements](#requirements)

[Instructions](#instructions)

[Releases](#releases)

[To-Do](#to-do)

## About the project
IoTHouse is a project to control a temperature/humidity module 
and a real time clock module through an arduino zero controller and visualize it on a display

## Requirements
<ul>
  <li>Arduino Zero</li>
  <li>SSD1306 Display</li>
  <li>RTC_DS3231 Module</li>
  <li>DHT11 Module</li>
  <li>4 Buttons</li>
 </ul>

## Instructions
Menu:
Press Button_1 to move cursor
Press Button_2 to select menu
To go back to the menu press Button_1

Date and Time:
To adjust time press Button_2 inside DateTime menu
Press Button_3 to decrease the value
Press Button_4 to increase the value
OBS! when you're adjusting there is no going back except for restarting

## Functions
changeMenu():
this is what makes the cursor move it can only move downwards 
until it reaches bottom and starts on top again
its an event callback for when Button_1 is pressed

selectEvent():
this function selects the highlighted menu option 
and adjusts the time and advances through all datetime steps
its used as an event callback for when Button_2 is pressed

addDate():
this function adds to input datetime and has limits 
so date steps doesnt exceed their maximum values
its used as an event callback for when Button_4 is pressed

## Releases

## To-Do
<ul>
  <li>Use millis() instead of delay() so the loop doesnt wait</li>
  <li>Add a return option to adjust datetime</li>
  <li>Use a Rotary Encoder for adjusting datetime instead of buttons</li>
  <li>Separate code into more files for readability</li>
 </ul>

