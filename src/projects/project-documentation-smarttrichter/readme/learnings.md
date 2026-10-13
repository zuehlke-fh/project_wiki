[⬅ Back to Main Page](README.md)

# Learnings: Smart Trichter

## Description
Throughout the project, we gained valuable insights, especially regarding planning, hardware compatibility, and sensor calibration.

## Content
### Key Takeaways:
- It is crucial to choose the right microcontroller in advance. In our case, we were lucky that all the pins fit together in the end—but that was more by chance than design. Better planning would have prevented potential issues.
- Components should not be built separately and then combined later. A well-thought-out overall concept saves time and avoids unnecessary iterations.
- Accurate measurements are essential: Had we measured everything more precisely, we wouldn’t have needed to print the housing twice.
-  Using the **Arduino IDE for a larger project like this was a bad idea**.  
   It lacks many useful features for bigger codebases, such as better debugging, refactoring tools, and project structuring.  
   → **Alternative:** For future projects, we would use **CLion or similar IDEs** that support C++ development in a more structured way.

### Biggest Challenges:
1. **Calibrating the Flow Sensor:**  
   The sensor had some fluctuations, and adjusting it to deliver realistic values for drinking speed was tricky.  
   → Solution: We spent around two hours repeatedly filling, measuring, and even testing by drinking ourselves to determine a reliable calibration factor.

2. **Optimizing Code for Flash Memory:**  
   We had to adjust our code to fit within the available memory of the microcontroller.

---

[⬅ Back to Main Page](README.md)
