# SimplifiedNAT

## Usage
build/simplified_NAT \<NAT file\> \<FLOW file\>
## Problem Description
Your program will read a list of NAT entries and a list of flows, and output the flows after they have been processed by your NAT.

## Assumptions
<ul>
	<li>When a flow matches multiple entries in the NAT due to wildcards, the output will be based on the following order:<br/>
	&nbsp&nbsp&nbsp  1) return exact NAT match for IP and port<br/>
	&nbsp&nbsp&nbsp  2) return exact match for IP but wildcard for port<br/>
	&nbsp&nbsp&nbsp  3) return exact match for port but wildcard for IP<br/>
	Example:<br/>
	NAT<br/>
	&nbsp&nbsp&nbsp  1:1,2:1<br/>
	&nbsp&nbsp&nbsp  1:*,2:2<br/>
	&nbsp&nbsp&nbsp  *:1,2:3<br/>
	Then incoming flows to "1:1" will translate to "2:1" even though 1:1 could also technically match 1:* and *:1. If line 1 was removed in NAT then incoming flows would translate to "2:2" instead and so on.</li>
	<li>All inputs are valid and formatted correctly (ie NAT file can only contain 1 comma per line, no white space in input files, etc)</li>
</ul>

## Tests
Run the ./run_tests.sh bash script to run tests
1) basic test for correctness
2) Empty NAT file
3) Empty FLOW file
4) priority tests
	1) Checks assumption 1 above
	2) Checks assumption 2 above
	3) Checks assumption 3 above

Each test folder includes a NAT and FLOW file as well as an EXPECTED file which includes my expected results. The program is run with the NAT and FLOW files and produces an OUTPUT file, which the bash script then compares with the EXPECTED file. Any differences will be outputed to the console.

## Thoughts
My initial thought was to use an unordered_map and store everything from the FLOW file as a key value pair. That way, when I read in the FLOWs line by line, I do an O(1) lookup to find the translated ip, port pair. In order to preserve the above ordering in case a FLOW matches multiple NAT entries, I can do a max of three lookups into my unordered_map and return the first match. First I would check for ip:port in my unordered_map, then ip:\* and finally \*:port. If none of them are in my unordered_map then I can safely say that the incoming flow does not match any entry in the NAT and output accordingly.

In terms of space complexity, each line of the NAT is stored once in the hash table, giving it a space complexity of O(N) where N is the number of entries in the NAT. I realized later on that this may not be the most space efficient as a lot of these entries in the NAT may be potentially unused. For example, if we consider a scenario where there are 10000 unique NAT entries but there is only 1 unique ip:port in the FLOW, then we would be storing a lot of unused input/output ip:port pairs. If I had more time, another approach I could consider would be the use of a cache with a least recently used eviction method. I assume this would work well as, in practice, packets for a specific application will be constantly going to a single ip:port pair within a certain time frame. So if one incoming packet is going to a specific ip:port pair, then I can expect more to follow, all going to the same address. This way, I would be able to maintain O(1) lookup speeds for cache hits while reducing the amount of space used.
