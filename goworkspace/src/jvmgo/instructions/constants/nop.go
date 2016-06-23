package constants

import "jvmgo/instructions/base"
import "jvmgo/rtda"

type NOP struct {
	base.NoOperandsInstruction
}

func (self *NOP) Execute(frame *rtda.Frame) {

}
