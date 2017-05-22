package main

import (
	"fmt"
	"os"
	"os/exec"
)

func main() {
	var (
		cmdOut []byte
		err    error
	)
	cmdName := "./TM"
	//cmdArgs := []string{"TM.txt", "1100"}
	cmdArgs := []string{"TM.txt", rawdata}
	if cmdOut, err = exec.Command(cmdName, cmdArgs...).Output(); err != nil {
		fmt.Fprintln(os.Stderr, "Problem with the Turing Machine: ", err)
		os.Exit(1)
	}
	sha := string(cmdOut)
	fmt.Println(rawdata[0], sha)
}
