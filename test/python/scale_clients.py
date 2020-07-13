#! /bin/python3
import subprocess
import time

def main():
    client_obs = []
    command = "../../cpp/build/client -s 127.0.0.1 -p 14000"
    for i in range(1,250):
        client_id = i
        num_msg = i*10
        command += " -n"+str(num_msg) + " -c"+str(client_id)
        client_obs.append(subprocess.Popen(command.split(' '),stdout=subprocess.PIPE))
    time.sleep(60);
    for i in (reversed(range(2,251))): 
        client_id = i
        num_msg = i*10
        command += " -n"+str(num_msg) + " -c"+str(client_id)
        client_obs.append(subprocess.Popen(command.split(' '),stdout=subprocess.PIPE))
    analyse = open("../../docs/analysis.csv","w")
    for client in client_obs:
        analyse.write(client.communicate()[0].decode())
    analyse.close()
    print("Done.")

if __name__ == "__main__":
    main()
