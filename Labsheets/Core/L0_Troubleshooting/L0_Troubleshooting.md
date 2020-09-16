# Trouble Shooting
<br>
<a href="#upload_issues">I can't upload my code to the Romi / My Romi won't connect.</a>
<br>
<a href="#serial_timing">The Timing of my Code changes with Serial.print()</a>
<br>
<a href="#switch_case">Switch case</a>



<a id="upload_issues"></a>
## I can't upload my code to the Romi / My Romi won't connect:


The Romi can sometimes lose its ability to connect to your computer.  Follow these steps to fix it.  On rare occasions, this problem can keep re-occuring, but this method can be repeated quickly and easily.


### Step1 
- Open the Arduino IDE
- Open the Blink Example
    - Go to File -> Examples -> Basics -> Blink
- Check that the board type `Arduino Leonardo` is selected
    - Go to Tools -> Board -> Select `Arduino Leonardo` from the list.

Paul

### Step 2 
- Enable `Show verbose output during: upload`
    - Go to File -> Preferences
    - Check the `upload` tick box, **so that it is ticked**.
    
<img src="https://github.com/paulodowd/EMATM0054_20_21/blob/master/images/verbose_upload.png?raw=true">

### Step 3


- Plug in your Romi to your computer via the USB cable.
- Your computer might say "device not recognised", don't let this trouble you - <a href="https://www.youtube.com/watch?v=fhXvGvH2ZCs">it is just a glorified filing cabinet</a>.
- Go to Tools -> Port and make a note of which ports are available.  You may have many or none.

### Step 4

- Press the reset button twice - **quickly** 
    - The reset button is circled red in the image below.
    - When you do this correctly, **you should see the built in LED fading** (cirlced green on Romi image).
    - Note that, thetes LED should **fade** in and out, not blink on and off.
    
<pre>
<table>
    <tr><td><img src="https://github.com/paulodowd/EMATM0054_20_21/blob/master/images/Romi_TopDown.jpg?raw=true" width="50%"></td><td><img src="https://github.com/paulodowd/EMATM0054_20_21/blob/master/images/reset_fade.gif?raw=true" ></td></tr>
</table>
</pre>

### Step 5

- **Whilst the LED is fading**
    - Go to Tools -> Port and select the newest Port which has shown up.
    - If there isn't a new port, select the one with the highest value (e.g., COM10)
    - Wait for the Fading LED to go out.  
    - If you check Tools -> Port again, this new port may have gone.  This is OK.
    
### Step 6

- Click the upload icon to upload the Blink code to your Romi.
- Wait for the code to compile. Wait until the Arduino IDE is searching for the port to connect to.  You will see something like the the white text appearing in the image below.
- **When you see the Arduino IDE searching for a port, press the reset button twice (quickly) again to get the fading LED**.

<img src="https://github.com/paulodowd/EMATM0054_20_21/blob/master/images/searching_for_port.png?raw=true">

### Step 7

Wait, and you should see text like the below image confirming the correct upload.  If not, try the process again.  Sometimes it can go wrong if you don't successsfully double-press the reset button quickly enough.  If, after a few attempts, you cannot get this to work, contact one of the teaching staff for the unit.

<img src="https://github.com/paulodowd/EMATM0054_20_21/blob/master/images/success.png?raw=true">

### Step 8

You may wish to:
- Share this technique with your peers if they need help.



<a id="serial_timing"></a>
# The Timing of my Code changes with Serial.print()

There is a known issue with `Serial.print` and `Serial.println` on the Romi.  You may find that your Romi becomes irratic or the timing of its behaviour changes when you unplug the USB cable.  

There are two steps required to fix this problem.  

## Step 1:

Include the following code at the top of your main program (the first tab in the Arduino IDE):


<pre>
<font color="#5e6d03">#include</font> <font color="#434f54">&lt;</font><font color="#000000">USBCore</font><font color="#434f54">.</font><font color="#000000">h</font><font color="#434f54">&gt;</font> &nbsp;&nbsp;&nbsp;<font color="#434f54">&#47;&#47; To fix serial print behaviour bug.</font>
<font color="#000000">u8</font> <font color="#000000">USB_SendSpace</font><font color="#000000">(</font><font color="#000000">u8</font> <font color="#000000">ep</font><font color="#000000">)</font><font color="#000000">;</font>
<font color="#5e6d03">#define</font> <font color="#000000">SERIAL_ACTIVE</font> <font color="#000000">(</font><font color="#000000">USB_SendSpace</font><font color="#000000">(</font><font color="#000000">CDC_TX</font><font color="#000000">)</font> <font color="#434f54">&gt;=</font> <font color="#000000">50</font><font color="#000000">)</font>

</pre>



Note that, if you use `Serial.print` or `Serial.println` within other source files, you may need to include this at the top of those files too.  Try to compile your code without it, and see if there are useful errors.  

## Step 2:

At every location you use `Serial.print` or `Serial.println`, you should wrap your code in the following way:
<pre>
<font color="#5e6d03">if</font><font color="#000000">(</font> <font color="#000000">SERIAL_ACTIVE</font> <font color="#000000">)</font> <font color="#000000">{</font>

 &nbsp;<b><font color="#d35400">Serial</font></b><font color="#434f54">.</font><font color="#d35400">print</font><font color="#000000">(</font><font color="#005c5f">&#34;An example&#34;</font><font color="#000000">)</font><font color="#000000">;</font>
 &nbsp;<b><font color="#d35400">Serial</font></b><font color="#434f54">.</font><font color="#d35400">println</font><font color="#000000">(</font><font color="#005c5f">&#34; of fixing the serial bug&#34;</font><font color="#000000">)</font><font color="#000000">;</font>
<font color="#000000">}</font>

</pre>

This will allow you to plug in and remove the USB cable without effecting the performance of your Robot.  

Note that, you can also use `if( SERIAL_ACTIVE ) { }` to create different behaviours depending on whether your robot is plugged into USB or not.




- Get back to work.
