package com.rusefi.output;

import com.rusefi.*;

import static com.rusefi.ToolUtil.EOL;

public abstract class BaseCHeaderConsumer implements ConfigurationConsumer {
    private static final String BOOLEAN_TYPE = "bool";
    private final StringBuilder content = new StringBuilder();

    private static String getHeaderText(ConfigField configField, int currentOffset, int bitIndex) {
        if (configField.isBit()) {
            String comment = "\t/**" + EOL + ConfigDefinition.packComment(configField.getCommentContent(), "\t") + "\toffset " + currentOffset + " bit " + bitIndex + " */" + EOL;
            return comment + "\t" + BOOLEAN_TYPE + " " + configField.getName() + " : 1 {};" + EOL;
        }

        String cEntry = ConfigDefinition.getComment(configField.getCommentContent(), currentOffset, configField.getUnits());

        String typeName = configField.getType();

        String autoscaleSpec = configField.autoscaleSpec();
        if (autoscaleSpec != null) {
            typeName = "scaled_channel<" + typeName + ", " + autoscaleSpec + ">";
        }

        if (!configField.isArray()) {
            // not an array
            cEntry += "\t" + typeName + " " + configField.getName();
            if (ConfigDefinition.needZeroInit && TypesHelper.isPrimitive(configField.getType())) {
                // we need this cast in case of enums
                cEntry += " = (" + configField.getType() + ")0";
            }
            cEntry += ";" + EOL;
        } else {
            cEntry += "\t" + typeName + " " + configField.getName() + "[" + configField.arraySizeVariableName + "];" + EOL;
        }
        return cEntry;
    }

    @Override
    public void handleEndStruct(ConfigStructure structure) {
        if (structure.comment != null) {
            content.append("/**" + EOL + ConfigDefinition.packComment(structure.comment, "") + EOL + "*/" + EOL);
        }

        content.append("// start of " + structure.name + EOL);
        content.append("struct " + structure.name + " {" + EOL);

        int currentOffset = 0;

        FieldIterator iterator = new FieldIterator(structure.cFields);
        for (int i = 0; i < structure.cFields.size(); i++) {
            iterator.start(i);
            content.append(getHeaderText(iterator.cf, currentOffset, iterator.bitState.get()));

            // todo: do we have a bug with conditional field offsets being wrong?
            currentOffset += iterator.cf.getSize(iterator.next);
            iterator.end();
        }

        content.append("\t/** total size " + currentOffset + "*/" + EOL);
        content.append("};" + EOL + EOL);
    }

    public StringBuilder getContent() {
        return content;
    }

    @Override
    public void startFile() {
    }
}
