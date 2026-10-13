[⬅ Back to Main Page](README.md)

# Result: Smart Trichter

## Description
The Smart Trichter works as planned for 0.3L drinks. All core functionalities, including measurement, data storage, and NFC login, have been implemented.

## Content
### Final Outcome:
- **Fully functional for 0.3L drinks**
- **Data can be stored and managed via NFC**
- **Guest mode was implemented using an additional chip**
- **Only missing feature:** Piezo buzzer (not included due to memory constraints)

### Flow Sensor Accuracy:
One key challenge is that the **flow sensor has a certain inaccuracy**.  
The measurement results can vary **depending on pressure and drinking speed**, which affects precision.  
Because of this, we had to implement a **threshold** to stabilize the readings.  

➡ **This means we can’t measure exactly 0.3L**, but instead values like **~250ml**, depending on the conditions.  
This limitation is inherent to the sensor and not a software issue.

![Smart Trichter Model](/projects/project-documentation-smarttrichter/images/Smart_Trichter.jpg)  
---

[⬅ Back to Main Page](README.md)
