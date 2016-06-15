package classfile

import "fmt"
import "unicode/utf16"

type ConstantUtf8Info struct {
	str string
}

func (self *ConstantUtf8Info) readInfo(reader *ClassReader) {
	length := uint32(reader.readUint16())
	bytes := reader.readBytes(length)
	self.str = decodeMUTF8(bytes)
}

// valid when bytes don't include null or supplementary characters
func decodeMUTF8(bytes []byte) string {
	return string(bytes)
}
