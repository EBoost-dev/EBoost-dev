eBoost Node - UBUNTU 16

Instructions for running an eBoost (EBST) node
Run steps 1-4 with sudo account.  Create new user account to run node. Best practice is to run node with basic user with no sudo access.

Recommended system configuration: 4GB RAM, 2 CPU, 20GB storage
Disk space usage: 2.2GB

Source code location:  https://github.com/EBoost-dev/EBoost-dev

1.	Start with Ubuntu 16	Commands below  ( You may also use CentOS or Ubuntu 18, see separate build instructions)
2.	Update linux	
    > sudo apt-get update
3.	Install Dependencies	
    > sudo apt-get install build-essential libtool autotools-dev automake pkg-config libssl-dev libevent-dev bsdmainutils ssh libboost-all-dev
4.	Update linux	
    > sudo apt-get update
5.	Github clone	
    > git clone https://github.com/EBoost-dev/EBoost-dev eboost
6.	Change dir	
    > cd eboost
7.	Fetch params	
    > ./autogen.sh
8.	Build	
    > ./configure
9.	Make PTX directory	
    > mkdir -p ~/.eboost-core

10.	Create Config	
    > cd .. && touch .eboost-core/eboost.conf
11.	Edit Config	
    > vi .eboost-core/eboost.conf
	#Add the following lines to the PTX.conf file:
rpcuser=yourrpcusername
rpcpassword=yoursecurerpcpassword
rpcbind=127.0.0.1
rpcport=5883
txindex=1
daemon=1
addnode=node1.minercity.org
addnode=node2.minercity.org
addnode=node3.minercity.org
12.	Create shell script startup-EBST.sh with command	
     "nohup /home/nodeuser/eboost/src/eboostd"
13.	Run shell script	
    > sh startup-EBST.sh
14.	Verify sync	
    > tail -f /home/nodeuser/.eboost-core/debug.txt
15.	Cronjob	Add startup-PTX.sh to crontab
    > crontab -e
eg.  @reboot sh /home/nodeuser/startup-EBST.sh
