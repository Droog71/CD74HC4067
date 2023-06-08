# CD74HC4067
CD74HC4067 16-Channel Analog Multiplexer/Demultiplexer for Wokwi<br />
Datasheet -> https://www.ti.com/lit/ds/symlink/cd74hc4067.pdf<br />
Wokwi -> https://wokwi.com/<br /><br />

The CD74HC4067 and CD74HCT4067 devices are digitally<br />
controlled analog switches that utilize silicon-gate CMOS<br />
technology to achieve operating speeds similar to LSTTL,<br />
with the low power consumption of standard CMOS<br />
integrated circuits.<br />
These analog multiplexers/demultiplexers control analog<br />
voltages that may vary across the voltage supply range.<br />
They are bidirectional switches thus allowing any analog<br />
input to be used as an output and vice-versa. The switches<br />
have low “on” resistance and low “off” leakages. In addition,<br />
these devices have an enable control which when high will<br />
disable all switches to their “off” state.<br />

<img src="https://i.imgur.com/J9hMKei.png" alt="Hardware" width="150" height="335">
<img src="https://i.imgur.com/RcSO67U.png" alt="Simulation" width="150" height="392"><br /><br />
<img src="https://i.imgur.com/wxUlurH.png" alt="Simulation" width="385" height="542"><br /><br /><br />

<h2>Examples</h2>
Multiplexing with potentiometers -> https://wokwi.com/projects/366560040454111233<br />
Multiplexing with DIP switches -> https://wokwi.com/projects/367007283236072449<br />
Driving LEDs -> https://wokwi.com/projects/366745528502490113<br />
Demultiplexing with analog values -> https://wokwi.com/projects/366644917441189889<br /><br /><br />

<h2>Attributes</h2>
When used for demultiplexing analog values on Wokwi, set the "analog_demux" attribute to 1 in diagram.json<br />

```
{
  "type": "board-cd74hc4067",
  "id": "mux1",
  "top": 103.19,
  "left": 61.05,
  "rotate": 90,
  "attrs": { "analog_demux": "1" }
}
```

This is only needed if you are connecting an analog device <br />
such as a potentiometer to the SIG / COM pin on the CD74HC4067 chip.<br /><br />
In this use case, the voltage produced by the potentiometer is directd<br />
to the selected channel of the CD74HC4067 chip.<br /><br />

When demultiplexing digital values on Wokwi for the purpose of driving LEDs<br />
or other digitally controlled devies, no special attributes are required.<br /><br />

When using the chip for multiplexing, no special attributes are required.<br /><br />


