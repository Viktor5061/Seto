# State graph generated by ./write_sg 4.2 (compiled 15-Oct-03 at 3:06 PM)
# from </home/vale/Scaricati/iccad95/pe-rcv-ifc.g> on 26-Jul-18 at 9:36 PM
.model /home/vale/Scaricati/iccad95/pe-rcv-ifc.g
.inputs  woq tack smsg ackrcvhs
.outputs  peack treq enrcv resreqrcv
.state graph 
s38 tack- s16
s38 smsg- s39
s22 peack- s27
s10 ackrcvhs- s12
s31 woq- s32
s7 tack- s45
s7 ackrcvhs+ s8
s3 woq- s4
s1 tack+ s2
s0 woq+ s44
s21 peack- s25
s21 enrcv+ s22
s6 treq- s7
s39 tack- s18
s37 treq+ s22
s37 peack- s43
s33 woq+ s41
s33 smsg+ s34
s19 treq+ s27
s8 tack- s9
s4 peack- s5
s2 peack+ s3
s40 treq- s38
s9 resreqrcv+ s10
s45 treq+ s27
s43 treq+ s27
s42 treq+ s25
s42 enrcv+ s43
s41 treq+ s1
s28 treq- s29
s23 peack- s26
s23 enrcv+ s24
s20 treq+ s24
s20 peack- s19
s17 treq+ s26
s17 enrcv+ s19
s12 resreqrcv- s13
s12 enrcv- s11
s44 treq+ s1
s30 peack+ s31
s29 tack- s30
s27 tack+ s6
s15 peack+ s38
s11 resreqrcv- s0
s35 smsg- s36
s24 peack- s27
s14 peack+ s40
s14 treq- s15
s13 enrcv- s0
s5 woq+ s28
s5 smsg+ s14
s36 treq+ s21
s36 peack- s42
s36 enrcv+ s37
s25 enrcv+ s27
s34 peack+ s35
s32 peack- s33
s26 enrcv+ s27
s18 treq+ s23
s18 peack- s17
s18 enrcv+ s20
s16 smsg- s18
.marking {s0}
.end
