/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 *
 * @format
 */

import React, {useLayoutEffect, useRef} from 'react';
import type {PropsWithChildren} from 'react';
import {
  Pressable,
  SafeAreaView,
  ScrollView,
  StatusBar,
  StyleSheet,
  Text,
  useColorScheme,
  View,
} from 'react-native';

import {
  Colors,
  DebugInstructions,
  Header,
  LearnMoreLinks,
  ReloadInstructions,
} from 'react-native/Libraries/NewAppScreen';
console.log("Importing NativeSampleModule from './tm/NativeSampleModule'");
import NativeSampleModule from './tm/NativeSampleModule';


type SectionProps = PropsWithChildren<{
  title: string;
}>;

function Section({children, title}: SectionProps): React.JSX.Element {
  const isDarkMode = useColorScheme() === 'dark';
  return (
    <View style={styles.sectionContainer}>
      <Text
        style={[
          styles.sectionTitle,
          {
            color: isDarkMode ? Colors.white : Colors.black,
          },
        ]}>
        {title}
      </Text>
      <Text
        style={[
          styles.sectionDescription,
          {
            color: isDarkMode ? Colors.light : Colors.dark,
          },
        ]}>
        {children}
      </Text>
    </View>
  );
}

function App(): React.JSX.Element {
  const isDarkMode = useColorScheme() === 'dark';
  const ref = useRef<View>( null );
  const [message, setMessage] = React.useState<string | null>("the quick brown fox jumps over the lazy dog");

  const backgroundStyle = {
    backgroundColor: isDarkMode ? Colors.darker : Colors.lighter,
  };

  useLayoutEffect(() => {
    const target = (ref.current as any)?.__nativeTag as number;
    NativeSampleModule.registerBoundsChangeCallback(target, ({oldRect, newRect}) => {
        if (oldRect.width !== newRect.width) {
            console.log("Width changed from", oldRect.width, "to", newRect.width);
            return false;
        }

        if (oldRect.height !== newRect.height) {
            console.log("Height changed from", oldRect.height, "to", newRect.height);
            return false;
        }

        return true;
    });

  }, []);

    useLayoutEffect(() => {
        ref.current?.measure((x, y, width, height, pageX, pageY) => {
            console.log("Measured dimensions", {x, y, width, height, pageX, pageY});
        });
    }, [message]);

  return (
    <SafeAreaView style={backgroundStyle}>
      <StatusBar
        barStyle={isDarkMode ? 'light-content' : 'dark-content'}
        backgroundColor={backgroundStyle.backgroundColor}
      />
      <ScrollView
        contentInsetAdjustmentBehavior="automatic"
        style={backgroundStyle}>
        <Header />
        <Pressable
            onPress={() => setMessage("the quick brown fox jumps over the lazy dogsdjflksdjfjsadlfjasldjflaksjdflkjasldkjflasjdfl;kjas;ldkfja;lsdjf;alksdjfla;sjdkflajsld;kfjals;kdjfkl;asjdflk;asjdlfjaskl;dfjasl;kdjflasdjflasjdfahsdkfjhasdklfhaksjdhfkjashdfkljashdkjlfhaskldhf")}
          >
            <View ref={ref}>
                <Section title="Cxx TurboModule">
                    {message}
                </Section>
            </View>

        </Pressable>
          <Section title="Step One">
            Edit <Text style={styles.highlight}>App.tsx</Text> to change this
            screen and then come back to see your edits.
          </Section>
          <Section title="See Your Changes">
            <ReloadInstructions />
          </Section>
          <Section title="Debug">
            <DebugInstructions />
          </Section>
          <Section title="Learn More">
            Read the docs to discover what to do next:
          </Section>
          <LearnMoreLinks />
      </ScrollView>
    </SafeAreaView>
  );
}

const styles = StyleSheet.create({
  sectionContainer: {
    marginTop: 32,
    paddingHorizontal: 24,
  },
  sectionTitle: {
    fontSize: 24,
    fontWeight: '600',
  },
  sectionDescription: {
    marginTop: 8,
    fontSize: 18,
    fontWeight: '400',
  },
  highlight: {
    fontWeight: '700',
  },
});

export default App;
